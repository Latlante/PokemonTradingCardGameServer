#include "stdlistenerwritter.h"
#include <QTextStream>
#include <QThread>

StdListenerWritter::StdListenerWritter(QObject *parent) :
    QObject(parent),
    m_stopThread(false)
{
    moveToThread(&m_thread);
    connect(&m_thread, &QThread::finished, this, &QObject::deleteLater);
    m_thread.start();
}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
void StdListenerWritter::startListening()
{

}

void StdListenerWritter::stopListening()
{
    m_stopThread = true;
}

void StdListenerWritter::write(QByteArray message)
{
    QTextStream sOut(stdout);

    sOut << message;
    sOut.flush();
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
void StdListenerWritter::listening()
{
    QTextStream sIn(stdin);

    while(m_stopThread == false)
    {
        QByteArray message = sIn.readLine();

        if(message.isEmpty() == false)
        {
            emit messageReceived(message);
        }

        QThread::msleep(100);
    }
}
