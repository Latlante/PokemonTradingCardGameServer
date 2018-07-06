#ifndef TCPSERVERCLIENTS_H
#define TCPSERVERCLIENTS_H

#include <QTcpServer>

class TcpServerClients : public QTcpServer
{
public:
    TcpServerClients();

    void start();

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // TCPSERVERCLIENTS_H
