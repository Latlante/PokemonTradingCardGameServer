#ifndef LOCALSERVERINSTANCE_H
#define LOCALSERVERINSTANCE_H

#include <QMap>
#include <QObject>

class QLocalServer;
class QLocalSocket;

class LocalServerInstance : public QObject
{
    Q_OBJECT
public:
    explicit LocalServerInstance(QObject *parent = nullptr);

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
    QLocalServer* m_tcpServer;
    QMap<QLocalSocket*, InfoInstance> m_mapThreadInstance;

    void executeRequest(QLocalSocket* client, const QByteArray &jsonReceived);
    QLocalSocket* socketFromUid(unsigned int uid);
};

#endif // LOCALSERVERINSTANCE_H
