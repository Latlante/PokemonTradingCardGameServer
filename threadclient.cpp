#include "threadclient.h"

#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTcpSocket>
#include <QVariant>

#include "authentification.h"
#include "instancemanager.h"
#include "Share/constantesshared.h"

ThreadClient::ThreadClient(int socketDescriptor, QObject *parent) :
    QThread(parent),
    m_socketDescriptor(socketDescriptor),
    m_tcpSocket(nullptr),
    m_user(""),
    m_uid(-1),
    m_token("")
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ThreadClient::run()
{
    qDebug() << "Starting thread: " << m_socketDescriptor;
    m_tcpSocket = new QTcpSocket();

    if(!m_tcpSocket->setSocketDescriptor(m_socketDescriptor))
    {
        qCritical() << "Error creation thread:" << m_tcpSocket->errorString();
        return;
    }

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ThreadClient::onReadyRead_TcpSocket, Qt::DirectConnection);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &ThreadClient::onDisconnected_TcpSocket, Qt::DirectConnection);
    connect(InstanceManager::instance(), &InstanceManager::readyRead, this, &ThreadClient::onReadyRead_InstanceManager);

    exec();
}

/************************************************************
*****       	  FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void ThreadClient::onReadyRead_TcpSocket()
{
    QByteArray message = m_tcpSocket->readAll();

    qDebug() << __PRETTY_FUNCTION__ << m_socketDescriptor << ", " << message;

    bool hasToRepond = true;
    QJsonObject jsonResponse;
    QJsonDocument jsonReceived = QJsonDocument::fromJson(message);

    if(!jsonReceived.isEmpty())
    {
        //if no authentify yet
        if(m_uid == -1)
        {
            jsonResponse = authentify(jsonReceived["name"].toString(),
                                      jsonReceived["password"].toString());
        }
        //authentification success
        else
        {
            QString tokenReceive = jsonReceived["token"].toString();

            //everything is good
            if(tokenReceive == m_token)
            {
                int phase = jsonReceived["phase"].toInt();
                qDebug() << __PRETTY_FUNCTION__ << "phase:" << phase;

                switch(phase)
                {
                case ConstantesShared::PHASE_ListOfGameForThePlayer:
                {
                    jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(m_uid);
                }
                    break;
                case ConstantesShared::PHASE_ListOfAllPlayers:
                {
                    QList<QString> listOfPlayers = Authentification::listOfAllUsers();
                    jsonResponse["players"] = QJsonArray::fromStringList(listOfPlayers);
                    jsonResponse["count"] = listOfPlayers.count();
                }
                    break;

                case ConstantesShared::PHASE_CreateANewGame:
                {
                    int uidOtherPlayer = jsonReceived["uidOtherPlayer"].toInt();
                    QString nameOfTheGame = jsonReceived["name"].toString();

                    if(InstanceManager::instance()->checkNameOfGameIsAvailable(nameOfTheGame))
                    {
                        unsigned int indexNewGame = InstanceManager::instance()->createNewGame(m_uid, uidOtherPlayer, nameOfTheGame);
                        jsonResponse["idGame"] = static_cast<int>(indexNewGame);
                    }
                    else
                    {
                        jsonResponse["idGame"] = 0;
                        jsonResponse["error"] = "One game has already that name";
                    }
                }
                    break;

                case ConstantesShared::PHASE_RemoveAGame:
                {
                    int uidGame = jsonReceived["uidGame"].toInt();
                    bool success = InstanceManager::instance()->removeGame(uidGame);

                    jsonResponse["result"] = success;
                }
                    break;

                //INSTANCES
                case ConstantesShared::PHASE_SelectCards:
                case ConstantesShared::PHASE_InitReady:
                case ConstantesShared::PHASE_MoveACard:
                {
                    hasToRepond = false;

                    //get information
                    unsigned int uidGame = jsonReceived["uidGame"].toInt();

                    //add name player in json
                    QJsonObject objectReceived = jsonReceived.object();
                    objectReceived["namePlayer"] = m_user;

                    //remove data useless
                    objectReceived.remove("token");
                    objectReceived.remove("uidGame");

                    //transfer to the game
                    if(InstanceManager::instance()->write(uidGame, QJsonDocument(objectReceived).toJson(QJsonDocument::Compact)))
                    {
                        qDebug() << __PRETTY_FUNCTION__ << "Write OK";
                    }
                    else
                    {
                        qDebug() << __PRETTY_FUNCTION__ << "Write error";
                    }
                }
                    break;

                default:
                    const QString error = "error: phase does not exist";
                    jsonResponse["result"] = "ko";
                    jsonResponse["error"] = error;
                    qCritical() << error;
                }
            }
            //wrong token
            else
            {
                const QString error = "wrong token";
                jsonResponse["result"] = "ko";
                jsonResponse["error"] = error;
                qCritical() << error;
            }

        }

    }
    else
    {
        const QString error = "error during the creation of the json document";
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = error;
        qCritical() << error;
    }

    if(hasToRepond == true)
    {
        m_tcpSocket->write(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }
}

void ThreadClient::onDisconnected_TcpSocket()
{
    m_tcpSocket->deleteLater();
    exit(0);
}

void ThreadClient::onReadyRead_InstanceManager(unsigned int uidGame, QByteArray message)
{
    qDebug() << __PRETTY_FUNCTION__ << "Message recu du process," << uidGame << message;

    m_tcpSocket->write(message);
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject ThreadClient::authentify(QString user, QString password)
{
    QJsonObject jsonResponse;
    Authentification auth;

    if(auth.checkUser(user, password))
    {
        //set data
        m_user = auth.user();
        m_uid = auth.uid();
        m_token = auth.token();

        //answer
        jsonResponse["result"] = "ok";
        jsonResponse["token"] = m_token;
        jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(m_uid);
    }
    else
    {
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = "authentification failed";
    }

    return jsonResponse;
}

QJsonArray ThreadClient::jsonArrayOfGamesForUidPlayer(int uidPlayer)
{
    QJsonArray jsonListIdGames;
    QList<unsigned int> listUidGames = InstanceManager::instance()->listUidGamesFromUidPlayer(uidPlayer);
    foreach(unsigned int uidGame, listUidGames)
    {
        QJsonObject jsonGame;

        jsonGame["uid"] = static_cast<int>(uidGame);
        jsonGame["name"] = InstanceManager::instance()->nameOfTheGameFromUidGame(uidGame);

        jsonListIdGames.append(jsonGame);
    }

    return jsonListIdGames;
}
