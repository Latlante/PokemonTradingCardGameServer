#ifndef TCPSERVERINSTANCE_H
#define TCPSERVERINSTANCE_H

#include <QTcpServer>

class ThreadInstance;

class TcpServerInstance : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServerInstance();

    void start();
    bool isRunning();

    bool newMessage(unsigned int uid, QByteArray message);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void newInstanceConnected(int);
    void instanceDisconnected(int);

private slots:
    void onInstanceAuthentified_ThreadInstance(unsigned int uid);

private:
    bool m_isRunning;
    QMap<unsigned int, ThreadInstance*> m_mapThreadInstance;

};

#endif // TCPSERVERINSTANCE_H
