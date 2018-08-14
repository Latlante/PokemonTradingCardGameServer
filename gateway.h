#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>

class TcpServerClients;
class TcpServerInstance;

class Gateway : public QObject
{
    Q_OBJECT
public:
    explicit Gateway(QObject *parent = nullptr);
    virtual ~Gateway() override;

    bool startServers();

signals:

private slots:
    void onWriteToInstance_serverClients(unsigned int uidGame, QByteArray message);
    void onWriteToClient_serverInstances(unsigned int uidGame, QByteArray message);

private:
    TcpServerClients* m_serverClients;
    TcpServerInstance* m_serverInstance;
};

#endif // GATEWAY_H
