#ifndef THREADCLIENT_H
#define THREADCLIENT_H

#include <QThread>
//#include <QObject>

class QTcpSocket;
class QTimer;

class ThreadClient : public QThread
{
    Q_OBJECT
public:
    explicit ThreadClient(int socketDescriptor, QObject *parent = nullptr);
    ~ThreadClient() override;

    void newMessage(QByteArray message);

public slots:
    void onReadyRead_InstanceManager(unsigned int uidGame, QByteArray message);

protected:
    void run() override;

signals:
    void clientAuthentified(unsigned int);
    void newUserConnected(int,QString);
    void userDisconnected(int);
    void writeToInstance(unsigned int,QByteArray);

private slots:
    void onReadyRead_TcpSocket();
    void onDisconnected_TcpSocket();


    void onTimeOut_timerWritting();

private:
    int m_socketDescriptor;
    QTcpSocket* m_tcpSocket;
    quint16 m_sizeAnswerAsynchrone;

    QTimer *m_timerWritting;
    QList<QByteArray> m_listMessageToSend;

    QString m_user;
    unsigned int m_uid;
    QString m_token;

    void executeRequest(const QJsonDocument &jsonReceived);
    QJsonObject authentify(QString user, QString password);

    QJsonArray jsonArrayOfGamesForUidPlayer(unsigned int uidPlayer);
};

#endif // THREADCLIENT_H
