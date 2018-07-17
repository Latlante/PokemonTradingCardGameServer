#include "historicalnotifications.h"

#include "abstractnotification.h"

HistoricalNotifications::HistoricalNotifications(QObject *parent) : QObject(parent)
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

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
