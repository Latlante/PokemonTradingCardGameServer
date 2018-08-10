#include "threadinstance.h"

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QTimer>
#include <QVariant>

ThreadInstance::ThreadInstance(int socketDescriptor, QObject *parent) :
    QThread(parent),
    m_socketDescriptor(socketDescriptor),
    m_tcpSocket(nullptr),
    m_sizeAnswerAsynchrone(0),
    m_timerWritting(nullptr),
    m_listMessageToSend({})
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void ThreadInstance::run()
{
    qDebug() << "Starting thread instance: " << m_socketDescriptor;

    moveToThread(QApplication::instance()->thread());

    m_tcpSocket = new QTcpSocket();

    if(!m_tcpSocket->setSocketDescriptor(m_socketDescriptor))
    {
        qCritical() << "Error creation thread:" << m_tcpSocket->errorString();
        return;
    }

    m_timerWritting = new QTimer();
    connect(m_timerWritting, &QTimer::timeout, this, &ThreadInstance::onTimeOut_timerWritting, Qt::DirectConnection);
    m_timerWritting->start(200);

    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &ThreadInstance::onReadyRead_TcpSocket, Qt::DirectConnection);
    connect(m_tcpSocket, &QTcpSocket::disconnected, this, &ThreadInstance::onDisconnected_TcpSocket, Qt::DirectConnection);

    exec();
}

/************************************************************
*****       	  FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void ThreadInstance::onReadyRead_TcpSocket()
{
    //init the answer
    QByteArray responseSerialize;
    QDataStream requestToRead(m_tcpSocket);

    if(m_sizeAnswerAsynchrone == 0)
    {
        //Check we have the minimum to start reading
        if(m_tcpSocket->bytesAvailable() < sizeof(quint16))
            return;

        //Check we have all the answer
        requestToRead >> m_sizeAnswerAsynchrone;
    }

    if(m_tcpSocket->bytesAvailable() < m_sizeAnswerAsynchrone)
        return;

    //From here, we have everything, so we can get all the message
    requestToRead >> responseSerialize;
    QJsonParseError jsonError;
    QJsonDocument docNotif = QJsonDocument::fromJson(responseSerialize, &jsonError);

    if(jsonError.error == QJsonParseError::NoError)
    {
        emit messageReceived(docNotif);
        m_sizeAnswerAsynchrone = 0;
    }
    else
    {
        QJsonObject jsonResponse;
        const QString error = "error during the creation of the json document";
        jsonResponse["success"] = "ko";
        jsonResponse["error"] = error;
        qCritical() << error;

        m_listMessageToSend.append(QJsonDocument(jsonResponse).toJson(QJsonDocument::Compact));
    }

}

void ThreadInstance::onDisconnected_TcpSocket()
{
    qDebug() << __PRETTY_FUNCTION__;
    emit instanceDisconnected(m_socketDescriptor);

    delete m_timerWritting;
    m_tcpSocket->deleteLater();
    exit(0);
}

void ThreadInstance::onTimeOut_timerWritting()
{
    if(m_listMessageToSend.count() > 0)
    {
        QByteArray message = m_listMessageToSend.takeFirst();
        QByteArray requestToSend;
        QDataStream in(&requestToSend, QIODevice::WriteOnly);

        in << static_cast<quint16>(message.length());
        in << message;

        m_tcpSocket->write(requestToSend);
    }
}
