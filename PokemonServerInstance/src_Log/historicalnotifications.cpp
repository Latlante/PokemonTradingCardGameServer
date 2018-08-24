#include "historicalnotifications.h"
#include <QDebug>

#include "src_Log/log.h"

//#include "abstractnotification.h"

HistoricalNotifications::HistoricalNotifications()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
int HistoricalNotifications::addNewNotification(AbstractNotification *notif)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + " " + QString::number(count()));

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

QJsonObject HistoricalNotifications::buildJsonNotificationFrom(unsigned int index, const QString &namePlayer)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + QString::number(index) + " / " + QString::number(count()));

    QJsonObject objToReturn;
    objToReturn["indexBegin"] = static_cast<int>(index);
    objToReturn["indexEnd"] = m_listNotifications.count();

    if(index < static_cast<unsigned int>(m_listNotifications.count()))
    {
        for(int i=index;i<m_listNotifications.count();++i)
        {
            if(m_listNotifications[i]->namePlayer() == namePlayer)
                objToReturn[QString::number(i)] = m_listNotifications[i]->messageJsonForOwner();
            else
                objToReturn[QString::number(i)] = m_listNotifications[i]->messageJsonForOthers();
        }
    }

    return objToReturn;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
