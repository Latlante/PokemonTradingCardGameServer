#ifndef THREADINSTANCE_H
#define THREADINSTANCE_H

#include <QThread>
#include <QJsonDocument>

class QTcpSocket;
class QTimer;

class ThreadInstance : public QThread
{
    Q_OBJECT
public:
    ThreadInstance(int socketDescriptor, QObject* parent = nullptr);

    void newMessage(QByteArray message);

protected:
    void run() override;

signals:
    void instanceAuthentified(unsigned int);
    void instanceDisconnected(int);
    void messageReceived(QJsonDocument);

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

    void executeRequest(const QJsonDocument &jsonReceived);
};

#endif // THREADINSTANCE_H
