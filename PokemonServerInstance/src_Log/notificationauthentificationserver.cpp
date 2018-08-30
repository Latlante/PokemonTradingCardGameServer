#include "notificationauthentificationserver.h"

NotificationAuthentificationServer::NotificationAuthentificationServer(const QString &namePlayer, const QString &uidGame, const QString &nameGame, const QString &namePlayer1, const QString &namePlayer2) :
    AbstractNotification(ConstantesShared::PHASE_None, namePlayer),
    m_uidGame(uidGame),
    m_nameGame(nameGame),
    m_namePlayer1(namePlayer1),
    m_namePlayer2(namePlayer2)
{

}

NotificationAuthentificationServer::~NotificationAuthentificationServer()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationAuthentificationServer::messageJsonForOwner()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["uidGame"] = m_uidGame;
    jsonResponse["nameGame"] = m_nameGame;
    jsonResponse["namePlayer1"] = m_namePlayer1;
    jsonResponse["namePlayer2"] = m_namePlayer2;

    return jsonResponse;
}

QJsonObject NotificationAuthentificationServer::messageJsonForOthers()
{
    return QJsonObject();
}
