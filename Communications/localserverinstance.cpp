#include "localserverinstance.h"

#include <QDataStream>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>

LocalServerInstance::LocalServerInstance(QObject *parent) :
    QObject(parent),
    m_tcpServer(new QLocalServer(this)),
    m_mapThreadInstance()
{
    connect(m_tcpServer, &QLocalServer::newConnection, this, &LocalServerInstance::onNewConnection_TcpServer);
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
bool LocalServerInstance::start()
{
    bool success = false;

    if(m_tcpServer)
    {
        if(m_tcpServer->listen("instancePokemon"))
        {
            qDebug() << "Server instance listening...";
            success = true;
        }
        else
        {
            qDebug() << "Server instance not started: " << m_tcpServer->errorString();
        }
    }

    return success;
}

bool LocalServerInstance::isRunning()
{
    bool running = false;

    if(m_tcpServer)
        running = m_tcpServer->isListening();

    return running;
}

bool LocalServerInstance::newMessage(unsigned int uid, QByteArray message)
{
    bool success = false;

    QLocalSocket* client = socketFromUid(uid);
    qDebug() << __PRETTY_FUNCTION__ << uid << message << client;
    if(client != nullptr)
    {
        qDebug() << __PRETTY_FUNCTION__ << "2";
        QByteArray requestToSend;
        QDataStream in(&requestToSend, QIODevice::WriteOnly);

        in << static_cast<quint16>(message.length());
        in << message;

        qint64 byteWritten = client->write(requestToSend);
        if(byteWritten == requestToSend.length())
        {
            qDebug() << __PRETTY_FUNCTION__ << "message sent" << byteWritten << "bytes";
            success = true;
        }
        else
            qCritical() << __PRETTY_FUNCTION__ << "error for sending, byteWritten:" << byteWritten << ", message length:" << message.length();
    }
    else
        qCritical() << __PRETTY_FUNCTION__ << "instance is nullptr";

    return success;
}

/************************************************************
*****             FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void LocalServerInstance::onNewConnection_TcpServer()
{
    qDebug() << __PRETTY_FUNCTION__;
    QLocalSocket* newClient = m_tcpServer->nextPendingConnection();
    m_mapThreadInstance.insert(newClient, { 0, 0 });

    connect(newClient, &QLocalSocket::readyRead, this, &LocalServerInstance::onReadyRead_Instance);
    connect(newClient, &QLocalSocket::disconnected, this, &LocalServerInstance::onDisconnected_Instance);

    emit newInstanceConnected(newClient->socketDescriptor());
}

void LocalServerInstance::onReadyRead_Instance()
{
    qDebug() << __PRETTY_FUNCTION__;

        //init the answer
    QLocalSocket* client = qobject_cast<QLocalSocket*>(sender());
    while(client->bytesAvailable() > 0)
    {
        QByteArray responseSerialize;
        QDataStream requestToRead(client);

        if(m_mapThreadInstance.contains(client) == false)
        {
            qCritical() << __PRETTY_FUNCTION__ << "client is not in the list";
            return;
        }

        if(m_mapThreadInstance[client].sizeBuffer == 0)
        {
            //Check we have the minimum to start reading
            if(client->bytesAvailable() < sizeof(quint16))
                return;

            //Check we have all the answer
            requestToRead >> m_mapThreadInstance[client].sizeBuffer;
        }

        if(client->bytesAvailable() < m_mapThreadInstance[client].sizeBuffer)
            return;

        //From here, we have everything, so we can get all the message
        requestToRead >> responseSerialize;

        executeRequest(client, responseSerialize);
        m_mapThreadInstance[client].sizeBuffer = 0;
    }
}

void LocalServerInstance::onDisconnected_Instance()
{
    qDebug() << __PRETTY_FUNCTION__;
    QLocalSocket* client = qobject_cast<QLocalSocket*>(sender());
    if(client)
    {
        emit instanceDisconnected(client->socketDescriptor());
        m_mapThreadInstance.remove(client);
        client->deleteLater();
    }
}

/************************************************************
*****             FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void LocalServerInstance::executeRequest(QLocalSocket* client, const QByteArray &jsonReceived)
{
    qDebug() << __PRETTY_FUNCTION__ << "message received:" << jsonReceived;

    if(jsonReceived.count(';') >= 1)
    {
        const QString owner = QString(jsonReceived).section(';', 0, 0);

        if(owner == "boot")
        {
            const QByteArray dataSerialize = jsonReceived.split(';')[1];
            const QJsonDocument jsonData = QJsonDocument::fromJson(dataSerialize);

            //Authentification
            if((jsonData.object().contains("phase") == false) &&
                    (jsonData.object().contains("uidGame") == true) &&
                    (jsonData.object().contains("nameGame") == true) &&
                    (jsonData.object().contains("namePlayer1") == true) &&
                    (jsonData.object().contains("namePlayer2") == true))
            {
                unsigned int uidGame = jsonData["uidGame"].toString().toUInt();
                const QString nameGame = jsonData["nameGame"].toString();
                const QString namePlayer1 = jsonData["namePlayer1"].toString();
                const QString namePlayer2 = jsonData["namePlayer2"].toString();
                if(uidGame > 0)
                {
                    if(m_mapThreadInstance.contains(client) == true)
                        m_mapThreadInstance[client].uid = uidGame;
                    else
                        qCritical() << __PRETTY_FUNCTION__ << "client is not in the map for uidGame" << uidGame;
                    emit instanceAuthentified(client->socketDescriptor(), uidGame, nameGame, namePlayer1, namePlayer2);
                }
                else
                    qWarning() << __PRETTY_FUNCTION__ << "error during authentification of the instance: " << jsonReceived;
            }
            else
            {
                qCritical() << __PRETTY_FUNCTION__ << "data missing for initialization";
            }
        }
        else
        {
            unsigned int uid = m_mapThreadInstance.value(client).uid;
            emit writeToClient(uid, jsonReceived);
        }
    }
}

QLocalSocket* LocalServerInstance::socketFromUid(unsigned int uid)
{
    QLocalSocket *socket = nullptr;
    int indexLoop = 0;

    QList<QLocalSocket*> listKeys = m_mapThreadInstance.keys();
    while((socket == nullptr) && (indexLoop < listKeys.count()))
    {
        if(m_mapThreadInstance.value(listKeys[indexLoop]).uid == uid)
            socket = listKeys[indexLoop];

        indexLoop++;
    }

    return socket;
}
