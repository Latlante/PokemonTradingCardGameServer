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
    int index = -1;

    if(notif != nullptr)
    {
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", phase=" + QString::number(notif->phase()) + ", index=" + QString::number(count()));

        m_listNotifications.append(notif);

        index = m_listNotifications.indexOf(notif);
    }

    return index;
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
    QJsonObject objToReturn = QJsonObject();

    Log::instance()->write(QString(__PRETTY_FUNCTION__) + QString::number(index) + " / " + QString::number(count()) + " for " + namePlayer.toLatin1());

    if(index < static_cast<unsigned int>(m_listNotifications.count()))
    {
        for(int i=index;i<m_listNotifications.count();++i)
        {
            if(m_listNotifications[i]->namePlayer() == namePlayer)
            {
                //security, add info only there is something in
                if(m_listNotifications[i]->messageJsonForOwner() != QJsonObject())
                    objToReturn[QString::number(i)] = m_listNotifications[i]->messageJsonForOwner();
            }
            else
            {
                if(m_listNotifications[i]->messageJsonForOthers() != QJsonObject())
                    objToReturn[QString::number(i)] = m_listNotifications[i]->messageJsonForOthers();
            }
        }

        //add index only if necessary => if there is data to send
        if(objToReturn.isEmpty() == false)
        {
            objToReturn["indexBegin"] = static_cast<int>(index);
            objToReturn["indexEnd"] = m_listNotifications.count();
        }
    }

    return objToReturn;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
