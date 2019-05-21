#include "tcpserverclientmonothread.h"

#include <QDataStream>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "authentification.h"
#include "instancemanager.h"
#include "Share/constantesshared.h"

TcpServerClientMonoThread::TcpServerClientMonoThread(QObject *parent) :
    QObject(parent),
    m_tcpServer(new QTcpServer(this)),
    m_listClients()
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &TcpServerClientMonoThread::onNewConnection_TcpServer);
}

TcpServerClientMonoThread::~TcpServerClientMonoThread()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool TcpServerClientMonoThread::start()
{
    bool success = false;

    if(m_tcpServer)
    {
        if(m_tcpServer->listen(QHostAddress::Any, 23002))
        {
            qDebug() << "Server listening...";
            success = true;
        }
        else
        {
            qDebug() << "Server not started: " << m_tcpServer->errorString();
        }
    }

    return success;
}

bool TcpServerClientMonoThread::isRunning()
{
    bool running = false;

    if(m_tcpServer)
        running = m_tcpServer->isListening();

    return running;
}

bool TcpServerClientMonoThread::newMessage(unsigned int uid, QByteArray message)
{
    QTcpSocket* client = socketFromUid(uid);
    return newMessage(client, message);
}

bool TcpServerClientMonoThread::newMessage(QTcpSocket* client, QByteArray message)
{
    bool success = false;

    if(client != nullptr)
    {
        QByteArray requestToSend;
        QDataStream in(&requestToSend, QIODevice::WriteOnly);

        in << static_cast<quint16>(message.length());
        in << message;

        qint64 byteWritten = client->write(requestToSend);
        if(byteWritten == requestToSend.length())
            success = true;
        else
            qCritical() << __PRETTY_FUNCTION__ << "error for sending, byteWritten:" << byteWritten << ", message length:" << message.length();
    }
    else
        qCritical() << __PRETTY_FUNCTION__ << "client is nullptr";

    return success;
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
void TcpServerClientMonoThread::onNewConnection_TcpServer()
{
    qDebug() << __PRETTY_FUNCTION__;
    QTcpSocket* newClient = m_tcpServer->nextPendingConnection();
    m_listClients.insert(newClient, { "", 0, "", 0 });

    connect(newClient, &QTcpSocket::readyRead, this, &TcpServerClientMonoThread::onReadyRead_Client);
    connect(newClient, &QTcpSocket::disconnected, this, &TcpServerClientMonoThread::onDisconnected_Client);


}

void TcpServerClientMonoThread::onReadyRead_Client()
{
    qDebug() << __PRETTY_FUNCTION__;

    //init the answer
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    QByteArray responseSerialize;
    QDataStream requestToRead(client);

    if(m_listClients.contains(client) == false)
    {
        qCritical() << __PRETTY_FUNCTION__ << "client is not in the list";
        return;
    }

    if(m_listClients[client].sizeBuffer == 0)
    {
        //Check we have the minimum to start reading
        if(client->bytesAvailable() < sizeof(quint16))
            return;

        //Check we have all the answer
        requestToRead >> m_listClients[client].sizeBuffer;
    }

    if(client->bytesAvailable() < m_listClients[client].sizeBuffer)
        return;

    //From here, we have everything, so we can get all the message
    requestToRead >> responseSerialize;
    QJsonParseError jsonError;
    QJsonDocument docNotif = QJsonDocument::fromJson(responseSerialize, &jsonError);

    if(jsonError.error == QJsonParseError::NoError)
    {
        executeRequest(client, docNotif);
        m_listClients[client].sizeBuffer = 0;
    }
    else
    {
        InfoClient info = m_listClients.value(client);
        QJsonObject jsonResponse;
        const QString error = "error during the creation of the json document";
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = error;
        qCritical() << info.user << error;

        newMessage(client, QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }
}

void TcpServerClientMonoThread::onDisconnected_Client()
{
    qDebug() << __PRETTY_FUNCTION__;
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if(client)
    {
        emit userDisconnected(client->socketDescriptor());
        m_listClients.remove(client);
        client->deleteLater();
    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void TcpServerClientMonoThread::executeRequest(QTcpSocket *client, const QJsonDocument &jsonReceived)
{
    bool hasToRepond = true;
    QJsonObject jsonResponse;
    InfoClient info = m_listClients.value(client);

    qDebug() << __PRETTY_FUNCTION__ << "message received:" << jsonReceived.toJson(QJsonDocument::Compact);

    //if no authentify yet
    if(info.uid == 0)
    {
        jsonResponse = authentify(client,
                                  jsonReceived["name"].toString(),
                                  jsonReceived["password"].toString());
        jsonResponse["phase"] = ConstantesShared::PHASE_Identification;
    }
    //authentification success
    else
    {
        QString tokenReceive = jsonReceived["token"].toString();

        //everything is good
        if(tokenReceive == info.token)
        {
            int phase = jsonReceived["phase"].toInt();
            qDebug() << info.user << __PRETTY_FUNCTION__ << "phase:" << phase;

            switch(phase)
            {
            case ConstantesShared::PHASE_ListOfGameForThePlayer:
            {
                jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(info.uid);
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
                    unsigned int indexNewGame = InstanceManager::instance()->createNewGame(info.uid, uidOtherPlayer, nameOfTheGame, error);
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
                objectReceived["namePlayer"] = info.user;

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
                qCritical() << info.user << error;
            }
        }
        //wrong token
        else
        {
            const QString error = "wrong token";
            jsonResponse["success"] = "ko";
            jsonResponse["error"] = error;
            qCritical() << info.user << error;
        }

        jsonResponse["phase"] = jsonReceived["phase"];
    }

    if(hasToRepond == true)
    {
        //m_tcpSocket->write(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
        newMessage(client, QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }
}

QJsonObject TcpServerClientMonoThread::authentify(QTcpSocket *client, QString user, QString password)
{
    QJsonObject jsonResponse;
    Authentification auth;

    if((client != nullptr) && (auth.checkUser(user, password)))
    {
        //set data
        InfoClient info;
        info.user = auth.user();
        info.uid = auth.uid();
        info.token = auth.token();
        info.sizeBuffer = m_listClients[client].sizeBuffer;
        m_listClients[client] = info;

        //answer
        jsonResponse["success"] = "ok";
        jsonResponse["token"] = info.token;
        jsonResponse["games"] = jsonArrayOfGamesForUidPlayer(info.uid);
        emit newUserConnected(client->socketDescriptor(), info.user);
        //emit clientAuthentified(info.uid);
    }
    else
    {
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = "authentification failed";
    }

    return jsonResponse;
}

QJsonArray TcpServerClientMonoThread::jsonArrayOfGamesForUidPlayer(unsigned int uidPlayer)
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

QTcpSocket* TcpServerClientMonoThread::socketFromUid(unsigned int uid)
{
    QTcpSocket *socket = nullptr;
    int indexLoop = 0;

    QList<QTcpSocket*> listKeys = m_listClients.keys();
    while((socket == nullptr) && (indexLoop < listKeys.count()))
    {
        if(m_listClients.value(listKeys[indexLoop]).uid == uid)
            socket = listKeys[indexLoop];

        indexLoop++;
    }

    return socket;
}
