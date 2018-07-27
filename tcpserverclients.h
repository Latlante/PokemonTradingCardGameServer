#ifndef TCPSERVERCLIENTS_H
#define TCPSERVERCLIENTS_H

#include <QTcpServer>

class TcpServerClients : public QTcpServer
{
    Q_OBJECT
public:
    TcpServerClients();

    void start();
    bool isRunning();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void newUserConnected(int,QString);
    void userDisconnected(int);

private:
    bool m_isRunning;
};

#endif // TCPSERVERCLIENTS_H
