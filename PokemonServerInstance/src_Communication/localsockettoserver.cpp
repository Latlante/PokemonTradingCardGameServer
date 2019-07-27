#include "localsockettoserver.h"

#include <QDataStream>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
#include <QLocalSocket>
#include <QThread>
#include <QTime>
#include <QTimer>

#include "src_Log/log.h"

LocalSocketToServer::LocalSocketToServer(QObject *parent) :
    QObject(parent),
    m_socket(new QLocalSocket(this)),
    m_timeOut(10000),
    m_sizeAnswerAsynchrone(0)
{
    connect(m_socket, &QLocalSocket::connected, this, &LocalSocketToServer::onConnected_Socket);
    connect(m_socket, &QLocalSocket::readyRead, this, &LocalSocketToServer::onReadyRead_Socket);
}

LocalSocketToServer::~LocalSocketToServer()
{
    delete m_socket;
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int LocalSocketToServer::timeOut()
{
    return m_timeOut;
}

void LocalSocketToServer::setTimeOut(int timer)
{
    m_timeOut = timer;
}

bool LocalSocketToServer::tryToConnect()
{
    bool success = true;

    Log::instance()->write(QString(__PRETTY_FUNCTION__));

    QTimer timerTimeOut;
    timerTimeOut.setSingleShot(true);
    timerTimeOut.start(timeOut());

    QEventLoop loop;
    connect(m_socket, &QLocalSocket::connected, &loop, &QEventLoop::quit);
    connect(&timerTimeOut, &QTimer::timeout, &loop, &QEventLoop::quit);

    m_socket->abort();
    m_socket->connectToServer("instancePokemon");

    loop.exec();

    if(timerTimeOut.isActive())
    {
        timerTimeOut.stop();
        success = true;
    }

    return success;
}

bool LocalSocketToServer::write(QByteArray message)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__)+ " message: " + message);

    //init request
    QByteArray requestToSend;
    QDataStream dataToSend(&requestToSend, QIODevice::WriteOnly);
    dataToSend << static_cast<quint16>(message.length());
    dataToSend << message;

    //send the request
    qint64 bytesWritten = m_socket->write(requestToSend);
    Log::instance()->write(QString(__PRETTY_FUNCTION__)+ QString::number(bytesWritten) + " on " + QString::number(requestToSend.length()));

    return bytesWritten == requestToSend.length();
}

qint64 LocalSocketToServer::bytesAvailables()
{
    return m_socket->bytesAvailable();
}

QByteArray LocalSocketToServer::messageWaiting()
{
    return m_socket->readAll();
}

/************************************************************
*****			  FONCTIONS SLOT PRIVEES				*****
************************************************************/
void LocalSocketToServer::onConnected_Socket()
{
    //emit connected();
}

void LocalSocketToServer::onReadyRead_Socket()
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) +
                           ", bytes available: " + QString::number(m_socket->bytesAvailable()) +
                           ", size buffer: " + QString::number(m_sizeAnswerAsynchrone));

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

    m_sizeAnswerAsynchrone = 0;
    emit messageReceived(responseSerialize);
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + "end of the function" + QString::number(m_sizeAnswerAsynchrone));
}
