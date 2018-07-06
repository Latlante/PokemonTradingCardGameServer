#include "tcpserverclients.h"

#include <QDebug>
#include "threadclient.h"

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
    connect(client, &ThreadClient::finished, client, &ThreadClient::deleteLater);
    client->start();
}
