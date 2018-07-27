#include "tcpserverclients.h"

#include <QDebug>
#include <QThread>
#include "threadclient.h"
#include "instancemanager.h"

TcpServerClients::TcpServerClients() :
    QTcpServer(),
    m_isRunning(false)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void TcpServerClients::start()
{
    if(listen(QHostAddress::Any, 23002))
    {
        qDebug() << "Server listening...";
        m_isRunning = true;
    }
    else
    {
        qDebug() << "Server not started: " << errorString();
    }
}

bool TcpServerClients::isRunning()
{
    return m_isRunning;
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
void TcpServerClients::incomingConnection(qintptr socketDescriptor)
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
