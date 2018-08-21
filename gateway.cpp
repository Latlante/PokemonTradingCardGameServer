#include "gateway.h"

#include <QDebug>

#include "instancemanager.h"
#include "tcpserverclients.h"
#include "CommunicationInstance/tcpserverinstance.h"

Gateway::Gateway(QObject *parent) :
    QObject(parent),
    m_serverClients(new TcpServerClients()),
    m_serverInstance(new TcpServerInstance())
{
    connect(m_serverClients, &TcpServerClients::newUserConnected, this, &Gateway::newUserConnected);
    connect(m_serverClients, &TcpServerClients::userDisconnected, this, &Gateway::userDisconnected);
    connect(m_serverClients, &TcpServerClients::writeToInstance, this, &Gateway::onWriteToInstance_serverClients);

    connect(m_serverInstance, &TcpServerInstance::newInstanceConnected, this, &Gateway::newInstanceConnected);
    connect(m_serverInstance, &TcpServerInstance::instanceAuthentified, this, &Gateway::instanceAuthentified);
    connect(m_serverInstance, &TcpServerInstance::instanceDisconnected, this, &Gateway::instanceDisconnected);
    connect(m_serverInstance, &TcpServerInstance::writeToClient, this, &Gateway::onWriteToClient_serverInstances);
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
    QList<unsigned int> listUidPlayers = InstanceManager::instance()->listUidPlayersFromUidGame(uidGame);
    unsigned int currentUidPlayer = messageSplit[0].toUInt();

    foreach(unsigned int uidPlayer, listUidPlayers)
    {
        if(uidPlayer == currentUidPlayer)
            m_serverClients->newMessage(uidPlayer, messageSplit[1].toLatin1());
        else if(messageSplit.count() >= 2)
        {
            if(messageSplit[2].length() > 2)
                m_serverClients->newMessage(uidPlayer, messageSplit[2].toLatin1());
        }
    }
}
