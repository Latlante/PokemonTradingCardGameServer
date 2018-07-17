#include "notificationplayerisready.h"
#include <QMap>

NotificationPlayerIsReady::NotificationPlayerIsReady(bool everyoneIsReady, QMap<QString, bool> mapPlayersReady) :
    AbstractNotification(),
    m_everyoneIsReady(everyoneIsReady)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject NotificationPlayerIsReady::messageJsonForOwner()
{
    return QJsonObject();
}

QJsonObject NotificationPlayerIsReady::messageJsonForOthers()
{
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifPlayerIsReady);
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
