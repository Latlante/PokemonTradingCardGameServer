#ifndef TCPSERVERINSTANCE_H
#define TCPSERVERINSTANCE_H

#include <QTcpServer>

class TcpServerInstance : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServerInstance();

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

#endif // TCPSERVERINSTANCE_H
