#include "threadclient.h"

#include <QApplication>
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QTcpSocket>
#include <QTimer>
#include <QVariant>

#include "authentification.h"
#include "instancemanager.h"
#include "Share/constantesshared.h"

ThreadClient::ThreadClient(int socketDescriptor, QObject *parent) :
    QThread(parent),
    m_socketDescriptor(socketDescriptor),
    m_tcpSocket(nullptr),
    m_timerWritting(nullptr),
    m_listMessageToSend({}),
    m_user(""),
    m_uid(-1),
    m_token("")
{
    connect(this, &ThreadClient::writeToInstance, InstanceManager::instance(), &InstanceManager::write);
}

ThreadClient::~ThreadClient()
{
    /*if(m_timerWritting != nullptr)
    {
        delete m_timerWritting;
        m_timerWritting = nullptr;
    }*/
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ThreadClient::run()
{
    qDebug() << "Starting thread: " << m_socketDescriptor;

    moveToThread(QApplication::instance()->thread());

    m_tcpSocket = new QTcpSocket();

    if(!m_tcpSocket->setSocketDescriptor(m_socketDescriptor))
    {
        qCritical() << "Error creation thread:" << m_tcpSocket->errorString();
        return;
    }

    m_timerWritting = new QTimer();
    connect(m_timerWritting, &QTimer::timeout, this, &ThreadClient::onTimeOut_timerWritting, Qt::DirectConnection);
    m_timerWritting->start(200);

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ThreadClient::onReadyRead_TcpSocket, Qt::DirectConnection);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &ThreadClient::onDisconnected_TcpSocket, Qt::DirectConnection);
    connect(InstanceManager::instance(), &InstanceManager::readyRead, this, &ThreadClient::onReadyRead_InstanceManager, Qt::QueuedConnection);

    exec();
}

/************************************************************
*****       	  FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void ThreadClient::onReadyRead_TcpSocket()
{
    QByteArray message = m_tcpSocket->readAll();

    qDebug() << m_user << __PRETTY_FUNCTION__ << m_socketDescriptor << ", " << message;

    bool hasToRepond = true;
    QJsonObject jsonResponse;
    QJsonDocument jsonReceived = QJsonDocument::fromJson(message);

    if(!jsonReceived.isEmpty())
    {
        //if no authentify yet
        if(m_uid == 0)
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
                qDebug() << m_user << __PRETTY_FUNCTION__ << "phase:" << phase;

                switch(phase)
                {
                case ConstantesShared::PHASE_ListOfGameForThePlayer:
                {
                    jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(m_uid);
                }
                    break;
                case ConstantesShared::PHASE_ListOfAllPlayers:
                {
                    QJsonArray listOfPlayers = Authentification::listOfAllUsers();
                    jsonResponse["players"] = listOfPlayers;
                    jsonResponse["count"] = listOfPlayers.count();
                }
                    break;

                case ConstantesShared::PHASE_CreateANewGame:
                {
                    int uidOtherPlayer = jsonReceived["uidOtherPlayer"].toInt();
                    QString nameOfTheGame = jsonReceived["name"].toString();

                    if(InstanceManager::instance()->checkNameOfGameIsAvailable(nameOfTheGame))
                    {
						QString error = "";
                        unsigned int indexNewGame = InstanceManager::instance()->createNewGame(m_uid, uidOtherPlayer, nameOfTheGame, error);
                        jsonResponse["idGame"] = static_cast<int>(indexNewGame);
						
						if(indexNewGame == 0)
							jsonResponse["error"] = error;
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
                case ConstantesShared::PHASE_Attack_Retreat:
                case ConstantesShared::PHASE_SkipTheTurn:
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
                    /*if(InstanceManager::instance()->write(uidGame, QJsonDocument(objectReceived).toJson(QJsonDocument::Compact)))
                    {
                        qDebug() << m_user << __PRETTY_FUNCTION__ << "Write OK";
                    }
                    else
                    {
                        qCritical() << m_user << __PRETTY_FUNCTION__ << "Write error";
                    }*/
                    emit writeToInstance(uidGame, QJsonDocument(objectReceived).toJson(QJsonDocument::Compact));
                }
                    break;

                default:
                    const QString error = "error: phase does not exist";
                    jsonResponse["result"] = "ko";
                    jsonResponse["error"] = error;
                    qCritical() << m_user << error;
                }
            }
            //wrong token
            else
            {
                const QString error = "wrong token";
                jsonResponse["result"] = "ko";
                jsonResponse["error"] = error;
                qCritical() << m_user << error;
            }

        }

    }
    else
    {
        const QString error = "error during the creation of the json document";
        jsonResponse["result"] = "ko";
        jsonResponse["error"] = error;
        qCritical() << m_user << error;
    }

    if(hasToRepond == true)
    {
        //m_tcpSocket->write(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
        m_listMessageToSend.append(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }
}

void ThreadClient::onDisconnected_TcpSocket()
{
    qDebug() << m_user << __PRETTY_FUNCTION__;

    delete m_timerWritting;
    m_tcpSocket->deleteLater();
    exit(0);
}

void ThreadClient::onReadyRead_InstanceManager(unsigned int uidGame, QByteArray message)
{
    qDebug() << m_user << __PRETTY_FUNCTION__ << "Message recu du process," << uidGame << message;

    //check if we are concerned by the message
    if(InstanceManager::instance()->isInTheGame(uidGame, m_uid))
    {
        /*QJsonDocument jsonReceived = QJsonDocument::fromJson(message);

        if(!jsonReceived.isEmpty())
        {
            QJsonValue valuePhase = jsonReceived["phase"];

            if(!valuePhase.isNull())
            {
                int phase = valuePhase.toInt();

                //add uidGame in answer
                QJsonObject objReceived = jsonReceived.object();
                objReceived["uidGame"] = uidGame;

                switch(static_cast<ConstantesShared::GamePhase>(phase))
                {
                    //INSTANCES
                case ConstantesShared::PHASE_SelectCards:
                case ConstantesShared::PHASE_InitReady:
                case ConstantesShared::PHASE_MoveACard:
                case ConstantesShared::PHASE_Attack_Retreat:
                case ConstantesShared::PHASE_SkipTheTurn:
                    m_listMessageToSend.append(QJsonDocument(objReceived).toJson(QJsonDocument::Compact));
                    break;

                    //NOTIFICATIONS
                case ConstantesShared::PHASE_NotifCardMoved:
                {

                }
                    break;

                case ConstantesShared::PHASE_NotifPlayerIsReady:
                {
                    //same for all players
                    m_listMessageToSend.append(QJsonDocument(objReceived).toJson(QJsonDocument::Compact));
                }
                    break;

                case ConstantesShared::PHASE_NotifPokemonAttack:
                {

                }
                    break;
                }
            }
        }*/

        QList<QByteArray> listArguments = message.split(';');

        if(listArguments.count() >= 2)
        {
            const QString namePlayer = listArguments[0];
            const QByteArray messageOwner = listArguments[1];
            const QByteArray messageOthers = listArguments.count() >= 3 ? listArguments[2] : QByteArray();

            if(m_user == namePlayer)
            {
                qDebug() << m_user << __PRETTY_FUNCTION__ << "Envoi message propriétaire";
                m_listMessageToSend.append(messageOwner);
            }
            else if((messageOthers.isEmpty() == false) && (messageOthers != "{}"))
            {
                qDebug() << m_user << __PRETTY_FUNCTION__ << "Envoi message aux autres";
                m_listMessageToSend.append(messageOthers);
            }
        }
    }


    //m_tcpSocket->write(message);
    //m_listMessageToSend.append(message);

}

void ThreadClient::onTimeOut_timerWritting()
{
    if(m_listMessageToSend.count() > 0)
    {
        QByteArray message = m_listMessageToSend.takeFirst();
        m_tcpSocket->write(message);
    }
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

QJsonArray ThreadClient::jsonArrayOfGamesForUidPlayer(unsigned int uidPlayer)
{
    QJsonArray jsonListIdGames;
    QList<unsigned int> listUidGames = InstanceManager::instance()->listUidGamesFromUidPlayer(uidPlayer);
    foreach(unsigned int uidGame, listUidGames)
    {
        QJsonObject jsonGame;

        jsonGame["uid"] = static_cast<int>(uidGame);
        jsonGame["name"] = InstanceManager::instance()->nameOfTheGameFromUidGame(uidGame);
        jsonGame["opponent"] = InstanceManager::instance()->nameOpponentOfInGame(uidGame, uidPlayer);

        jsonListIdGames.append(jsonGame);
    }

    return jsonListIdGames;
}
