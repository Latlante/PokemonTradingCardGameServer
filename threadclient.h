#ifndef THREADCLIENT_H
#define THREADCLIENT_H

#include <QThread>

class QTcpSocket;

class ThreadClient : public QThread
{
    Q_OBJECT
public:
    explicit ThreadClient(int socketDescriptor, QObject *parent = nullptr);

signals:

protected:
    void run() override;

private slots:
    void onReadyRead_TcpSocket();
    void onDisconnected_TcpSocket();

    void onReadyRead_Process();

private:
    int m_socketDescriptor;
    QTcpSocket* m_tcpSocket;

    QString m_user;
    int m_uid;
    QString m_token;

    QJsonObject authentify(QString user, QString password);

    QJsonArray jsonArrayOfGamesForUidPlayer(int uidPlayer);
};

#endif // THREADCLIENT_H
