#include "historicalnotifications.h"

//#include "abstractnotification.h"

HistoricalNotifications::HistoricalNotifications()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int HistoricalNotifications::addNewNotification(AbstractNotification *notif)
{
    m_listNotifications.append(notif);

    return m_listNotifications.indexOf(notif);
}

unsigned int HistoricalNotifications::readPoint()
{
    return m_listNotifications.count();
}

QJsonObject HistoricalNotifications::buildJsonOwnerFrom(unsigned int index)
{
    QJsonObject objToReturn;

    if(index < static_cast<unsigned int>(m_listNotifications.count()))
    {
        for(int i=index;i<m_listNotifications.count();++i)
        {
            objToReturn[QString::number(index)] = m_listNotifications[i]->messageJsonForOwner();
        }
    }

    return objToReturn;
}

QJsonObject HistoricalNotifications::buildJsonOthersFrom(unsigned int index)
{
    QJsonObject objToReturn;

    if(index < static_cast<unsigned int>(m_listNotifications.count()))
    {
        for(int i=index;i<m_listNotifications.count();++i)
        {
            objToReturn[QString::number(index)] = m_listNotifications[i]->messageJsonForOthers();
        }
    }

    return objToReturn;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
