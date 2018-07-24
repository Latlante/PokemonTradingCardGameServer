#ifndef TCPSERVERCLIENTS_H
#define TCPSERVERCLIENTS_H

#include <QTcpServer>

class TcpServerClients : public QTcpServer
{
public:
    TcpServerClients();

    void start();
    bool isRunning();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    bool m_isRunning;
};

#endif // TCPSERVERCLIENTS_H
