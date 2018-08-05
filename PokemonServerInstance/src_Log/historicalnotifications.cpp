#include "historicalnotifications.h"
#include <QDebug>

//#include "abstractnotification.h"

HistoricalNotifications::HistoricalNotifications()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int HistoricalNotifications::addNewNotification(AbstractNotification *notif)
{
    qDebug() << __PRETTY_FUNCTION__ << count();

    m_listNotifications.append(notif);

    return m_listNotifications.indexOf(notif);
}

unsigned int HistoricalNotifications::readPoint()
{
    return m_listNotifications.count();
}

int HistoricalNotifications::count()
{
    return m_listNotifications.count();
}

QJsonObject HistoricalNotifications::buildJsonOwnerFrom(unsigned int index)
{
    qDebug() << __PRETTY_FUNCTION__ << index << "/" << count();

    QJsonObject objToReturn;
    objToReturn["indexBegin"] = static_cast<int>(index);
    objToReturn["indexEnd"] = m_listNotifications.count();

    if(index < static_cast<unsigned int>(m_listNotifications.count()))
    {
        for(int i=index;i<m_listNotifications.count();++i)
        {
            objToReturn[QString::number(i)] = m_listNotifications[i]->messageJsonForOwner();
        }
    }

    return objToReturn;
}

QJsonObject HistoricalNotifications::buildJsonOthersFrom(unsigned int index)
{
    qDebug() << __PRETTY_FUNCTION__ << index << "/" << count();

    QJsonObject objToReturn;
    objToReturn["indexBegin"] = static_cast<int>(index);
    objToReturn["indexEnd"] = m_listNotifications.count();

    if(index < static_cast<unsigned int>(m_listNotifications.count()))
    {
        for(int i=index;i<m_listNotifications.count();++i)
        {
            objToReturn[QString::number(i)] = m_listNotifications[i]->messageJsonForOthers();
        }
    }

    return objToReturn;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
