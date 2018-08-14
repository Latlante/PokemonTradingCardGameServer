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
