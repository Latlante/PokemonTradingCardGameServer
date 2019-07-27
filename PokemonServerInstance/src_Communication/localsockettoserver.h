#ifndef LOCALSOCKETTOSERVER_H
#define LOCALSOCKETTOSERVER_H

#include <QObject>

class QLocalSocket;

class LocalSocketToServer : public QObject
{
    Q_OBJECT
public:
    explicit LocalSocketToServer(QObject *parent = nullptr);
    ~LocalSocketToServer();

    int timeOut();
    void setTimeOut(int timer);

    bool tryToConnect();
    bool write(QByteArray message);
    qint64 bytesAvailables();
    QByteArray messageWaiting();

signals:
    void messageReceived(QByteArray);

private slots:
    void onConnected_Socket();
    void onReadyRead_Socket();

private:
    QLocalSocket* m_socket;
    int m_timeOut;

    quint16 m_sizeAnswerAsynchrone;
};

#endif // LOCALSOCKETTOSERVER_H
