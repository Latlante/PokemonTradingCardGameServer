#ifndef TCPSERVERCLIENTMONOTHREAD_H
#define TCPSERVERCLIENTMONOTHREAD_H

#include <QMap>
#include <QObject>

class QTcpServer;
class QTcpSocket;

class TcpServerClientMonoThread : public QObject
{
    Q_OBJECT
public:
    explicit TcpServerClientMonoThread(QObject *parent = nullptr);
    ~TcpServerClientMonoThread();

    bool start();
    bool isRunning();

    bool newMessage(unsigned int uid, QByteArray message);
    bool newMessage(QTcpSocket *client, QByteArray message);

signals:
    void newUserConnected(int,QString);
    void userDisconnected(int);
    void writeToInstance(unsigned int,QByteArray);

private slots:
    void onNewConnection_TcpServer();
    void onReadyRead_Client();
    void onDisconnected_Client();

private:
    struct InfoClient
    {
        QString user;
        unsigned int uid;
        QString token;
        quint16 sizeBuffer;
    };
    QTcpServer* m_tcpServer;
    QMap<QTcpSocket*, InfoClient> m_listClients;


    void executeRequest(QTcpSocket* client, const QJsonDocument &jsonReceived);
    QJsonObject authentify(QTcpSocket* client, QString user, QString password);

    QJsonArray jsonArrayOfGamesForUidPlayer(unsigned int uidPlayer);
    QTcpSocket* socketFromUid(unsigned int uid);
};

#endif // TCPSERVERCLIENTMONOTHREAD_H
