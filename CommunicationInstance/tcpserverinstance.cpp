#include "tcpserverinstance.h"

#include "threadinstance.h"

TcpServerInstance::TcpServerInstance() :
    QTcpServer(),
    m_isRunning(false)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void TcpServerInstance::start()
{
    if(listen(QHostAddress::Any, 23003))
    {
        qDebug() << "Server instance listening...";
        m_isRunning = true;
    }
    else
    {
        qDebug() << "Server instance not started: " << errorString();
    }
}

bool TcpServerInstance::isRunning()
{
    return m_isRunning;
}

bool TcpServerInstance::newMessage(unsigned int uid, QByteArray message)
{
    bool success = false;

    if((m_mapThreadInstance.key(uid, nullptr) != nullptr) && (message.length() > 0))
    {
        ThreadInstance* instance = m_mapThreadInstance.key(uid, nullptr);
        if(instance != nullptr)
        {
            instance->newMessage(message);
            success = true;
        }
    }

    return success;
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
void TcpServerInstance::incomingConnection(qintptr socketDescriptor)
{
    ThreadInstance* client = new ThreadInstance(socketDescriptor);
    //QThread* thread = new QThread();
    //client->moveToThread(thread);
    //connect(thread, &QThread::started, client, &ThreadClient::run);
    connect(client, &ThreadInstance::instanceAuthentified, this, &TcpServerInstance::onInstanceAuthentified_ThreadInstance);
    connect(client, &ThreadInstance::instanceDisconnected, this, &TcpServerInstance::instanceDisconnected);
    connect(client, &QThread::finished, client, &ThreadInstance::deleteLater);
    //connect(InstanceManager::instance(), &InstanceManager::readyRead, client, &ThreadClient::onReadyRead_InstanceManager, Qt::QueuedConnection);

    m_mapThreadInstance.insert(client, 0);

    client->start();
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void TcpServerInstance::onInstanceAuthentified_ThreadInstance(unsigned int uid)
{
    ThreadInstance* client = qobject_cast<ThreadInstance*>(sender());

    if(m_mapThreadInstance.contains(client) == true)
        m_mapThreadInstance[client] = uid;
    else
        qWarning() << __PRETTY_FUNCTION__ << "m_mapThreadInstance does not contains client " << uid;
}

void TcpServerInstance::onWriteToClient_ThreadInstance(QByteArray message)
{
    ThreadInstance* instance = qobject_cast<ThreadInstance*>(sender());
    const unsigned int uidGame = m_mapThreadInstance.value(instance, 0);

    if(uidGame > 0)
    {
        emit writeToClient(uidGame, message);
    }
    else
    {
        qWarning() << __PRETTY_FUNCTION__ << "uidGame = 0";
    }
}
