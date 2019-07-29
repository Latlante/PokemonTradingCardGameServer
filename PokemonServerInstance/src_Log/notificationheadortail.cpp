#include "notificationheadortail.h"

NotificationHeadOrTail::NotificationHeadOrTail(const QString &namePlayer, QList<unsigned short> headsOrTails) :
    AbstractNotification(ConstantesShared::PHASE_NotifHeadOrTail, namePlayer),
    m_headsOrTails(headsOrTails)
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

    QJsonArray arrayCoins;
    foreach(unsigned short coin, m_headsOrTails)
        arrayCoins.push_back(coin);

    jsonResponse["coins"] = arrayCoins;

    return jsonResponse;
}

QJsonObject NotificationHeadOrTail::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    QJsonArray arrayCoins;
    foreach(unsigned short coin, m_headsOrTails)
        arrayCoins.push_back(coin);

    jsonResponse["coins"] = arrayCoins;

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
