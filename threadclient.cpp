#include "threadclient.h"

#include <QApplication>
#include <QDataStream>
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
    m_sizeAnswerAsynchrone(0),
    m_timerWritting(nullptr),
    m_listMessageToSend({}),
    m_user(""),
    m_uid(0),
    m_token("")
{
}

ThreadClient::~ThreadClient()
{
    if(m_timerWritting != nullptr)
    {
        delete m_timerWritting;
        m_timerWritting = nullptr;
    }
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ThreadClient::newMessage(QByteArray message)
{
    if(message.length() > 0)
        m_listMessageToSend.append(message);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
void ThreadClient::run()
{
    qDebug() << "Starting thread client: " << m_socketDescriptor;

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

    exec();
}

/************************************************************
*****       	  FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void ThreadClient::onReadyRead_TcpSocket()
{
    //init the answer
    QByteArray responseSerialize;
    QDataStream requestToRead(m_tcpSocket);

    if(m_sizeAnswerAsynchrone == 0)
    {
        //Check we have the minimum to start reading
        if(m_tcpSocket->bytesAvailable() < sizeof(quint16))
            return;

        //Check we have all the answer
        requestToRead >> m_sizeAnswerAsynchrone;
    }

    if(m_tcpSocket->bytesAvailable() < m_sizeAnswerAsynchrone)
        return;

    //From here, we have everything, so we can get all the message
    requestToRead >> responseSerialize;
    QJsonParseError jsonError;
    QJsonDocument docNotif = QJsonDocument::fromJson(responseSerialize, &jsonError);

    if(jsonError.error == QJsonParseError::NoError)
    {
        executeRequest(docNotif);
        m_sizeAnswerAsynchrone = 0;
    }
    else
    {
        QJsonObject jsonResponse;
        const QString error = "error during the creation of the json document";
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = error;
        qCritical() << m_user << error;

        m_listMessageToSend.append(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }

}

void ThreadClient::onDisconnected_TcpSocket()
{
    qDebug() << m_user << __PRETTY_FUNCTION__;
    emit userDisconnected(m_socketDescriptor);

    delete m_timerWritting;
    m_tcpSocket->deleteLater();
    exit(0);
}

void ThreadClient::onTimeOut_timerWritting()
{
    if(m_listMessageToSend.count() > 0)
    {
        QByteArray message = m_listMessageToSend.takeFirst();
        QByteArray requestToSend;
        QDataStream in(&requestToSend, QIODevice::WriteOnly);

        in << static_cast<quint16>(message.length());
        in << message;

        m_tcpSocket->write(requestToSend);
    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void ThreadClient::executeRequest(const QJsonDocument &jsonReceived)
{
    bool hasToRepond = true;
    QJsonObject jsonResponse;

    qDebug() << __PRETTY_FUNCTION__ << "message received:" << jsonReceived.toJson(QJsonDocument::Compact);

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

                jsonResponse["success"] = success;
            }
                break;

            //INSTANCES
            case ConstantesShared::PHASE_GetAllInfoOnGame:
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
                objectReceived["uidPlayer"] = static_cast<int>(m_uid);

                //remove data useless
                objectReceived.remove("token");
                objectReceived.remove("uidGame");

                //transfer to the game
                emit writeToInstance(uidGame, QJsonDocument(objectReceived).toJson(QJsonDocument::Compact));
            }
                break;

            default:
                const QString error = "error: phase does not exist";
                jsonResponse["success"] = "ko";
                jsonResponse["error"] = error;
                qCritical() << m_user << error;
            }
        }
        //wrong token
        else
        {
            const QString error = "wrong token";
            jsonResponse["success"] = "ko";
            jsonResponse["error"] = error;
            qCritical() << m_user << error;
        }

    }

    if(hasToRepond == true)
    {
        //m_tcpSocket->write(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
        m_listMessageToSend.append(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }
}

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
        jsonResponse["success"] = "ok";
        jsonResponse["token"] = m_token;
        jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(m_uid);
        emit newUserConnected(m_socketDescriptor, m_user);
        emit clientAuthentified(m_uid);
    }
    else
    {
        jsonResponse["success"] = "ko";
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
