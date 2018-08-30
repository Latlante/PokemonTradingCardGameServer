#include "notificationnewgamecreated.h"

NotificationNewGameCreated::NotificationNewGameCreated(const QString &namePlayer, const QString &opponent) :
    AbstractNotification(ConstantesShared::PHASE_NotifNewGameCreated, namePlayer)
{

}

NotificationNewGameCreated::~NotificationNewGameCreated()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationNewGameCreated::messageJsonForOwner()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["opponent"] = m_opponent;

    return jsonResponse;
}

QJsonObject NotificationNewGameCreated::messageJsonForOthers()
{
    return QJsonObject();
}
