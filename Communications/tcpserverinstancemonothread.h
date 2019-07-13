#ifndef TCPSERVERINSTANCEMONOTHREAD_H
#define TCPSERVERINSTANCEMONOTHREAD_H

#include <QMap>
#include <QObject>

class QTcpServer;
class QTcpSocket;

class TcpServerInstanceMonoThread : public QObject
{
    Q_OBJECT
public:
    explicit TcpServerInstanceMonoThread(QObject *parent = nullptr);

    bool start();
    bool isRunning();

    bool newMessage(unsigned int uid, QByteArray message);

signals:
    void newInstanceConnected(int);
    void instanceAuthentified(int,unsigned int,const QString&,const QString&,const QString&);
    void instanceDisconnected(int);
    void writeToClient(unsigned int,QByteArray);

private slots:
    void onNewConnection_TcpServer();
    void onReadyRead_Instance();
    void onDisconnected_Instance();

private:
    struct InfoInstance
    {
        unsigned int uid;
        quint16 sizeBuffer;
    };
    QTcpServer* m_tcpServer;
    QMap<QTcpSocket*, InfoInstance> m_mapThreadInstance;

    void executeRequest(QTcpSocket* client, const QByteArray &jsonReceived);
    QTcpSocket* socketFromUid(unsigned int uid);
};

#endif // TCPSERVERINSTANCEMONOTHREAD_H
