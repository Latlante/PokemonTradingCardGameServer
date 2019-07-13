#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>

class TcpServerClients;
class TcpServerClientMonoThread;
class TcpServerInstanceMonoThread;

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = nullptr);
    virtual ~Gateway() override;

    bool startServers();

signals:
    void newUserConnected(int,QString);
    void userDisconnected(int);
    void newInstanceConnected(int);
    void instanceAuthentified(int,unsigned int,const QString&,const QString&,const QString&);
    void instanceDisconnected(int);

private slots:
    void onWriteToInstance_serverClients(unsigned int uidGame, QByteArray message);
    void onWriteToClient_serverInstances(unsigned int uidGame, QByteArray message);

private:
    //TcpServerClients* m_serverClients;
    TcpServerClientMonoThread* m_serverClients;
    TcpServerInstanceMonoThread* m_serverInstance;
};

#endif // GATEWAY_H
