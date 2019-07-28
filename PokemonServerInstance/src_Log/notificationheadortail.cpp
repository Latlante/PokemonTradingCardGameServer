#include "notificationheadortail.h"

NotificationHeadOrTail::NotificationHeadOrTail(const QString &namePlayer, bool headOrTail) :
    AbstractNotification(ConstantesShared::PHASE_NotifHeadOrTail, namePlayer),
    m_headOrTail(headOrTail)
{

}

NotificationHeadOrTail::~NotificationHeadOrTail()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationHeadOrTail::messageJsonForOwner()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["coin"] = m_headOrTail;

    return jsonResponse;
}

QJsonObject NotificationHeadOrTail::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["coin"] = m_headOrTail;

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
