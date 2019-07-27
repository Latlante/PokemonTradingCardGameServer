#include "gateway.h"

#include <QDebug>
#include <QJsonDocument>

#include "authentification.h"
#include "instancemanager.h"
#include "tcpserverclients.h"
//#include "CommunicationInstance/tcpserverinstance.h"
//#include "Communications/tcpserverinstancemonothread.h"
#include "Communications/localserverinstance.h"
#include "Communications/tcpserverclientmonothread.h"

Gateway::Gateway(QObject *parent) :
    QObject(parent),
    m_serverClients(new TcpServerClientMonoThread()),
    m_serverInstance(new LocalServerInstance())
{
    connect(m_serverClients, &TcpServerClientMonoThread::newUserConnected, this, &Gateway::newUserConnected);
    connect(m_serverClients, &TcpServerClientMonoThread::userDisconnected, this, &Gateway::userDisconnected);
    connect(m_serverClients, &TcpServerClientMonoThread::writeToInstance, this, &Gateway::onWriteToInstance_serverClients);

    connect(m_serverInstance, &LocalServerInstance::newInstanceConnected, this, &Gateway::newInstanceConnected);
    connect(m_serverInstance, &LocalServerInstance::instanceAuthentified, this, &Gateway::instanceAuthentified);
    connect(m_serverInstance, &LocalServerInstance::instanceDisconnected, this, &Gateway::instanceDisconnected);
    connect(m_serverInstance, &LocalServerInstance::writeToClient, this, &Gateway::onWriteToClient_serverInstances);
}

Gateway::~Gateway()
{
    delete m_serverClients;
    delete m_serverInstance;
}

/************************************************************
*****               FONCTIONS PUBLIQUES                 *****
************************************************************/
bool Gateway::startServers()
{
    bool success = false;
    if(InstanceManager::instance()->checkInstanceExeExists())
    {
        m_serverClients->start();
        m_serverInstance->start();

        success = m_serverClients->isRunning() && m_serverInstance->isRunning();
    }
    else
        qCritical() << __PRETTY_FUNCTION__ << "Instance exe not found";

    return success;
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void Gateway::onWriteToInstance_serverClients(unsigned int uidGame, QByteArray message)
{
    qDebug() << __PRETTY_FUNCTION__ << uidGame << message;
    m_serverInstance->newMessage(uidGame, message);
}

void Gateway::onWriteToClient_serverInstances(unsigned int uidGame, QByteArray message)
{
    qDebug() << __PRETTY_FUNCTION__ << uidGame << message;
    QList<QString> messageSplit = QString(message).split(";");

    if(messageSplit.count() % 2 == 0)
    {
        //fill a map with all message
        QMap<QString, QJsonDocument> mapMessagesJson;
        for(int i=0;i<messageSplit.count();i+=2)
        {
            const QString namePlayer = messageSplit[i];
            const QString jsonText = messageSplit[i+1];

            QJsonDocument doc = QJsonDocument::fromJson(jsonText.toLatin1());
            if(doc.isEmpty() == false)
                mapMessagesJson.insert(namePlayer, doc);
        }

        //send the message corresponding to the player
        QList<unsigned int> listUidPlayers = InstanceManager::instance()->listUidPlayersFromUidGame(uidGame);

        foreach(unsigned int uidPlayer, listUidPlayers)
        {
            const QString namePlayer = Authentification::namePlayerFromUid(uidPlayer);

            if(mapMessagesJson.contains(namePlayer))
                m_serverClients->newMessage(uidPlayer, mapMessagesJson.value(namePlayer).toJson(QJsonDocument::Compact));
            else
                m_serverClients->newMessage(uidPlayer, mapMessagesJson.value("").toJson(QJsonDocument::Compact));
        }
    }
    else
        qCritical() << __PRETTY_FUNCTION__ << "structure of message is incorrect:" << messageSplit.count() << "elements found";

}
