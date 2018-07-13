#include "tcpserverclients.h"

#include <QDebug>
#include <QThread>
#include "threadclient.h"
#include "instancemanager.h"

TcpServerClients::TcpServerClients()
{

}

void TcpServerClients::start()
{
    if(listen(QHostAddress::Any, 23002))
    {
        qDebug() << "Server listening...";
    }
    else
    {
        qDebug() << "Server not started: " << errorString();
    }
}

void TcpServerClients::incomingConnection(qintptr socketDescriptor)
{
    ThreadClient* client = new ThreadClient(socketDescriptor);
    QThread* thread = new QThread();
    client->moveToThread(thread);
    connect(thread, &QThread::started, client, &ThreadClient::run);
    connect(thread, &QThread::finished, client, &ThreadClient::deleteLater);
    //connect(InstanceManager::instance(), &InstanceManager::readyRead, client, &ThreadClient::onReadyRead_InstanceManager, Qt::QueuedConnection);

    thread->start();
}
