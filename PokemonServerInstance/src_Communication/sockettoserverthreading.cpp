#include "sockettoserverthreading.h"

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

SocketToServerThreading::SocketToServerThreading(QObject *parent) :
    QThread(parent),
    m_socket(nullptr),
    m_timeOut(10000),
    m_sizeAnswerAsynchrone(0)
{

}

SocketToServerThreading::~SocketToServerThreading()
{
    if(m_socket)
    {
        delete m_socket;
        m_socket = nullptr;
    }
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int SocketToServerThreading::timeOut()
{
    return m_timeOut;
}

void SocketToServerThreading::setTimeOut(int timer)
{
    m_timeOut = timer;
}

bool SocketToServerThreading::tryToConnect()
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

void SocketToServerThreading::write(const QByteArray &message)
{
    emit messageEmitted(message);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
void SocketToServerThreading::run()
{
    m_socket = new QTcpSocket();

    connect(m_socket, &QTcpSocket::connected, this, &SocketToServerThreading::onConnected_Socket, Qt::DirectConnection);
    connect(this, &SocketToServerThreading::messageEmitted, this, &SocketToServerThreading::onMessageEmitted, Qt::DirectConnection);
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketToServerThreading::onReadyRead_Socket, Qt::DirectConnection);

    if(tryToConnect())
    {
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "connection success");
        emit connected();
        exec();
    }
    else
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "connection failed");
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void SocketToServerThreading::onConnected_Socket()
{
    //emit connected();
    Log::instance()->write(QString(__PRETTY_FUNCTION__));
}

void SocketToServerThreading::onMessageEmitted(const QByteArray& message)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + QTime::currentTime().toString("hh:mm:ss.zzz").toLatin1() + " message: " + message);

    //init request
    QByteArray requestToSend;
    QDataStream dataToSend(&requestToSend, QIODevice::WriteOnly);
    dataToSend << static_cast<quint16>(message.length());
    dataToSend << message;

    //send the request
    qint64 bytesWritten = m_socket->write(requestToSend);

    if(bytesWritten == requestToSend.length())
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + QString::number(bytesWritten) + " on " + QString::number(requestToSend.length()));
    else
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + "error:" + QString::number(bytesWritten) + " written instead of " + QString::number(requestToSend.length()));
}

void SocketToServerThreading::onReadyRead_Socket()
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
