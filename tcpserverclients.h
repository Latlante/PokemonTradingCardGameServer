#ifndef TCPSERVERCLIENTS_H
#define TCPSERVERCLIENTS_H

#include <QTcpServer>

class ThreadClient;

class TcpServerClients : public QTcpServer
{
    Q_OBJECT
public:
    TcpServerClients();

    void start();
    bool isRunning();

    bool newMessage(unsigned int uid, QByteArray message);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void newUserConnected(int,QString);
    void userDisconnected(int);
    void writeToInstance(unsigned int,QByteArray);

private slots:
    void onClientAuthentified_ThreadClient(unsigned int uid);

private:
    bool m_isRunning;
    QMap<ThreadClient*, unsigned int> m_mapThreadClients;
};

#endif // TCPSERVERCLIENTS_H
