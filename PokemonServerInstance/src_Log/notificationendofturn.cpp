#include "notificationendofturn.h"

NotificationEndOfTurn::NotificationEndOfTurn(const QString &oldPlayer, const QString &newPlayer) :
    AbstractNotification(ConstantesShared::PHASE_NotifEndOfTurn),
    m_nameOldPlayer(oldPlayer),
    m_nameNewPlayer(newPlayer)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationEndOfTurn::messageJsonForOwner()
{
    return messageJsonForOthers();
}

QJsonObject NotificationEndOfTurn::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["endOfTurn"] = m_nameOldPlayer;
    jsonResponse["newTurn"] = m_nameNewPlayer;

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
