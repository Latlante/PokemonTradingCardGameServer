#ifndef SOCKETTOSERVER_H
#define SOCKETTOSERVER_H

#include <QObject>

class QTcpSocket;

class SocketToServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketToServer(QObject *parent = nullptr);
    ~SocketToServer();

    int timeOut();
    void setTimeOut(int timer);

    bool tryToConnect();
    bool write(QByteArray message);

signals:
    void messageReceived(QByteArray);

private slots:
    void onConnected_Socket();
    void onReadyRead_Socket();

private:
    QTcpSocket* m_socket;
    int m_timeOut;

    quint16 m_sizeAnswerAsynchrone;

};

#endif // SOCKETTOSERVER_H
