#include "notificationnewgamecreated.h"

NotificationNewGameCreated::NotificationNewGameCreated(int uid, const QString &nameGame, const QString &namePlayer, const QString &opponent) :
    AbstractNotification(ConstantesShared::PHASE_NotifNewGameCreated, namePlayer),
    m_uid(uid),
    m_nameGame(nameGame),
    m_opponent(opponent)
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
    return QJsonObject();
}

QJsonObject NotificationNewGameCreated::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["uidGame"] = m_uid;
    jsonResponse["nameGame"] = m_nameGame;
    jsonResponse["opponent"] = namePlayer();

    return jsonResponse;
}
