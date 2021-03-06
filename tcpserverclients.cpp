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

bool TcpServerClients::newMessage(unsigned int uid, QByteArray message)
{
    bool success = false;

    if((m_mapThreadClients.key(uid, nullptr) != nullptr) && (message.length() > 0))
    {
        ThreadClient* client = m_mapThreadClients.key(uid, nullptr);
        if(client != nullptr)
        {
            client->newMessage(message);
            success = true;
        }
    }

    return success;
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
    connect(client, &ThreadClient::clientAuthentified, this, &TcpServerClients::onClientAuthentified_ThreadClient);
    connect(client, &ThreadClient::newUserConnected, this, &TcpServerClients::newUserConnected);
    connect(client, &ThreadClient::userDisconnected, this, &TcpServerClients::userDisconnected);
    connect(client, &ThreadClient::writeToInstance, this, &TcpServerClients::writeToInstance);
    connect(client, &QThread::finished, client, &ThreadClient::deleteLater);
    //connect(InstanceManager::instance(), &InstanceManager::readyRead, client, &ThreadClient::onReadyRead_InstanceManager, Qt::QueuedConnection);

    m_mapThreadClients.insert(client, 0);

    client->start();
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void TcpServerClients::onClientAuthentified_ThreadClient(unsigned int uid)
{
    ThreadClient* client = qobject_cast<ThreadClient*>(sender());

    if(m_mapThreadClients.contains(client) == true)
        m_mapThreadClients[client] = uid;
    else
        qWarning() << __PRETTY_FUNCTION__ << "m_mapThreadClients does not contains client " << uid;
}
