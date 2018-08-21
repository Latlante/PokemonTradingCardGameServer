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
    void instanceAuthentified(int,unsigned int,const QString&,const QString&,const QString&);
    void instanceDisconnected(int);
    void writeToClient(QByteArray);

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

    void executeRequest(const QByteArray &jsonReceived);
};

#endif // THREADINSTANCE_H
