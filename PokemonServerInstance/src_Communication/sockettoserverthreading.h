#ifndef SOCKETTOSERVERTHREADING_H
#define SOCKETTOSERVERTHREADING_H

#include <QThread>

class QTcpSocket;

class SocketToServerThreading : public QThread
{
    Q_OBJECT
public:
    explicit SocketToServerThreading(QObject *parent = nullptr);
    ~SocketToServerThreading() override;

    int timeOut();
    void setTimeOut(int timer);

    bool tryToConnect();
    void write(const QByteArray& message);

signals:
    void connected();
    void messageEmitted(const QByteArray&);
    void messageReceived(QByteArray);

protected:
    void run() override;

private slots:
    void onConnected_Socket();
    void onMessageEmitted(const QByteArray& message);
    void onReadyRead_Socket();

private:
    QTcpSocket* m_socket;
    int m_timeOut;

    quint16 m_sizeAnswerAsynchrone;
};

#endif // SOCKETTOSERVERTHREADING_H
