#ifndef THREADCLIENT_H
#define THREADCLIENT_H

//#include <QThread>
#include <QObject>

class QTcpSocket;
class QTimer;

class ThreadClient : public QObject
{
    Q_OBJECT
public:
    explicit ThreadClient(int socketDescriptor, QObject *parent = nullptr);
    ~ThreadClient();

public slots:
    void onReadyRead_InstanceManager(unsigned int uidGame, QByteArray message);

public slots:
    void run();

private slots:
    void onReadyRead_TcpSocket();
    void onDisconnected_TcpSocket();


    void onTimeOut_timerWritting();

private:
    int m_socketDescriptor;
    QTcpSocket* m_tcpSocket;

    QTimer *m_timerWritting;
    QList<QByteArray> m_listMessageToSend;

    QString m_user;
    int m_uid;
    QString m_token;

    QJsonObject authentify(QString user, QString password);

    QJsonArray jsonArrayOfGamesForUidPlayer(int uidPlayer);
};

#endif // THREADCLIENT_H
