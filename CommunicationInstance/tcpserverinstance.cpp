#include "tcpserverinstance.h"

TcpServerInstance::TcpServerInstance() :
    QTcpServer(),
    m_isRunning(false)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void TcpServerInstance::start()
{
    if(listen(QHostAddress::Any, 23003))
    {
        qDebug() << "Server instance listening...";
        m_isRunning = true;
    }
    else
    {
        qDebug() << "Server instance not started: " << errorString();
    }
}

bool TcpServerInstance::isRunning()
{
    return m_isRunning;
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
void TcpServerInstance::incomingConnection(qintptr socketDescriptor)
{
    ThreadClient* client = new ThreadClient(socketDescriptor);
    //QThread* thread = new QThread();
    //client->moveToThread(thread);
    //connect(thread, &QThread::started, client, &ThreadClient::run);
    connect(client, &ThreadClient::newUserConnected, this, &TcpServerClients::newUserConnected);
    connect(client, &ThreadClient::userDisconnected, this, &TcpServerClients::userDisconnected);
    connect(client, &QThread::finished, client, &ThreadClient::deleteLater);
    //connect(InstanceManager::instance(), &InstanceManager::readyRead, client, &ThreadClient::onReadyRead_InstanceManager, Qt::QueuedConnection);

    client->start();
}
