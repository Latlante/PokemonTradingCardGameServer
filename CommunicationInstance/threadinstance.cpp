#include "threadinstance.h"

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
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
void ThreadInstance::newMessage(QByteArray message)
{
    if(message.length() > 0)
        m_listMessageToSend.append(message);
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
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
    qDebug() << __PRETTY_FUNCTION__ << "data incoming (" << m_tcpSocket->bytesAvailable() << ")";

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

    executeRequest(responseSerialize);
    m_sizeAnswerAsynchrone = 0;
}

void ThreadInstance::onDisconnected_TcpSocket()
{
    qDebug() << __PRETTY_FUNCTION__;
    emit instanceDisconnected(m_socketDescriptor);

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

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void ThreadInstance::executeRequest(const QByteArray &jsonReceived)
{
    qDebug() << __PRETTY_FUNCTION__ << "message received:" << jsonReceived;

    if(jsonReceived.count(';') >= 1)
    {
        const QString owner = QString(jsonReceived).section(';', 0, 0);

        if(owner == "boot")
        {
            const QByteArray dataSerialize = jsonReceived.split(';')[1];
            const QJsonDocument jsonData = QJsonDocument::fromJson(dataSerialize);

            //Authentification
            if((jsonData.object().contains("phase") == false) &&
                    (jsonData.object().contains("uidGame") == true) &&
                    (jsonData.object().contains("nameGame") == true) &&
                    (jsonData.object().contains("namePlayer1") == true) &&
                    (jsonData.object().contains("namePlayer2") == true))
            {
                unsigned int uidGame = static_cast<unsigned int>(jsonData["uidGame"].toInt());
                const QString nameGame = jsonData["nameGame"].toString();
                const QString namePlayer1 = jsonData["namePlayer1"].toString();
                const QString namePlayer2 = jsonData["namePlayer2"].toString();
                if(uidGame > 0)
                    emit instanceAuthentified(m_socketDescriptor, uidGame, nameGame, namePlayer1, namePlayer2);
                else
                    qWarning() << __PRETTY_FUNCTION__ << "error during authentification of the instance: " << jsonReceived;
            }
            else
            {
                qCritical() << __PRETTY_FUNCTION__ << "data missing for initialization";
            }
        }
        else
        {
            emit writeToClient(jsonReceived);
        }
    }


}
