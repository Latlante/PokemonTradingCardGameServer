#include "sockettoserver.h"

#include <QDataStream>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include <QTime>
#include <QTimer>

#include "src_Log/log.h"

SocketToServer::SocketToServer(QObject *parent) :
    QObject(parent),
    m_socket(new QTcpSocket(this)),
    m_timeOut(10000),
    m_sizeAnswerAsynchrone(0)
{
    connect(m_socket, &QTcpSocket::connected, this, &SocketToServer::onConnected_Socket);
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketToServer::onReadyRead_Socket);
}

SocketToServer::~SocketToServer()
{
    delete m_socket;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int SocketToServer::timeOut()
{
    return m_timeOut;
}

void SocketToServer::setTimeOut(int timer)
{
    m_timeOut = timer;
}

bool SocketToServer::tryToConnect()
{
    bool success = false;
    const QString ipAddress = "127.0.0.1";
    const int port = 23003;

    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + ipAddress + ":" + QString::number(port));

    if((ipAddress != "") && (port > 0))
    {
        QTimer timerTimeOut;
        timerTimeOut.setSingleShot(true);
        timerTimeOut.start(timeOut());

        QEventLoop loop;
        connect(m_socket, &QTcpSocket::connected, &loop, &QEventLoop::quit);
        connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);

        m_socket->abort();
        m_socket->connectToHost(QHostAddress(ipAddress), port);

        loop.exec();

        if(timerTimeOut.isActive())
        {
            timerTimeOut.stop();
            success = true;
        }
    }

    return success;
}

bool SocketToServer::write(QByteArray message)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + QTime::currentTime().toString("hh:mm:ss.zzz").toLatin1() + " message: " + message);

    //init request
    QByteArray requestToSend;
    QDataStream dataToSend(&requestToSend, QIODevice::WriteOnly);
    dataToSend << static_cast<quint16>(message.length());
    dataToSend << message;

    //send the request
    qint64 bytesWritten = m_socket->write(requestToSend);

    return bytesWritten == requestToSend.length();
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void SocketToServer::onConnected_Socket()
{
    //emit connected();
}

void SocketToServer::onReadyRead_Socket()
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__));

    //init the answer
    QByteArray responseSerialize;
    QDataStream requestToRead(m_socket);

    if(m_sizeAnswerAsynchrone == 0)
    {
        //Check we have the minimum to start reading
        if(m_socket->bytesAvailable() < sizeof(quint16))
            return;

        //Check we have all the answer
        requestToRead >> m_sizeAnswerAsynchrone;
    }

    if(m_socket->bytesAvailable() < m_sizeAnswerAsynchrone)
        return;

    //From here, we have everything, so we can get all the message
    requestToRead >> responseSerialize;

    emit messageReceived(responseSerialize);
    m_sizeAnswerAsynchrone = 0;

}
