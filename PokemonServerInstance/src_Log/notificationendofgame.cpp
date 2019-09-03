#include "notificationendofgame.h"

NotificationEndOfGame::NotificationEndOfGame(const QString &namePlayerWinner, const QString &namePlayerLoser) :
    AbstractNotification(ConstantesShared::PHASE_NotifEndOfGame),
    m_namePlayerWinner(namePlayerWinner),
    m_namePlayerLoser(namePlayerLoser)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationEndOfGame::messageJsonForOwner()
{
    return messageJsonForOthers();
}

QJsonObject NotificationEndOfGame::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["winner"] = m_namePlayerWinner;
    jsonResponse["loser"] = m_namePlayerLoser;

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
