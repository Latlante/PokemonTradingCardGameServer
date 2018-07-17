#include "notificationendofturn.h"

NotificationEndOfTurn::NotificationEndOfTurn(const QString &oldPlayer, const QString &newPlayer) :
    AbstractNotification(),
    m_nameOldPlayer(oldPlayer),
    m_nameNewPlayer(newPlayer)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject NotificationEndOfTurn::messageJsonForOwner()
{
    return QJsonObject();
}

QJsonObject NotificationEndOfTurn::messageJsonForOthers()
{
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifEndOfTurn);
    jsonResponse["endOfTurn"] = m_nameOldPlayer;
    jsonResponse["newTurn"] = m_nameNewPlayer;

    return jsonResponse;
}
