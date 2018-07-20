#include "stdlistenerwritter.h"
#include <QTextStream>

StdListenerWritter::StdListenerWritter(QObject *parent) :
    QObject(parent),
    m_stopThread(false)
{
    moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, this, &StdListenerWritter::listening);
    connect(&m_thread, &QThread::finished, this, &QObject::deleteLater);

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void StdListenerWritter::startListening()
{
    m_thread.start();
}

void StdListenerWritter::stopListening()
{
    m_stopThread = true;
}

void StdListenerWritter::write(QByteArray message)
{
    emit logReceived(QString(__PRETTY_FUNCTION__) + ", message" + message);

    QTextStream sOut(stdout);

    sOut << message;
    sOut.flush();
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void StdListenerWritter::listening()
{
    emit logReceived("StdListenerWritter: Start listening");

    QTextStream sIn(stdin);

    while(m_stopThread == false)
    {
        QString message = sIn.readLine();

        if(message.isEmpty() == false)
        {
            emit logReceived("StdListenerWritter: Message received");
            emit messageReceived(message);
        }

        QThread::msleep(100);
    }

    emit logReceived("StdListenerWritter: Stop listening");
}
