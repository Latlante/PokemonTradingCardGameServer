#include "notificationplayerisready.h"

NotificationPlayerIsReady::NotificationPlayerIsReady(bool everyoneIsReady, QMap<QString, bool> mapPlayersReady) :
    AbstractNotification(ConstantesShared::PHASE_NotifPlayerIsReady),
    m_everyoneIsReady(everyoneIsReady),
    m_mapPlayersReady(mapPlayersReady)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationPlayerIsReady::messageJsonForOwner()
{
    return messageJsonForOthers();
}

QJsonObject NotificationPlayerIsReady::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["everyoneIsReady"] = m_everyoneIsReady;

    QJsonArray arrayPlayers;
    for(QMap<QString,bool>::iterator it=m_mapPlayersReady.begin();it!=m_mapPlayersReady.end();++it)
    {
        QJsonObject objPlayer;
        objPlayer["namePlayer"] = it.key();
        objPlayer["ready"] = it.value();

        arrayPlayers.append(objPlayer);
    }
    jsonResponse["players"] = arrayPlayers;

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
