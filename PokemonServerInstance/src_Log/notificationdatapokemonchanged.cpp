#include "notificationdatapokemonchanged.h"
#include <QMap>

NotificationDataPokemonChanged::NotificationDataPokemonChanged(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int lifeLeft, QMap<unsigned int, bool> mapAttacksAvailable, QList<unsigned int> listIdEnergies) :
    AbstractNotification(),
    m_packet(packet),
    m_indexCard(indexCard),
    m_lifeLeft(lifeLeft),
    m_mapAttacksAvailable(mapAttacksAvailable),
    m_listIdEnergies(listIdEnergies)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject NotificationDataPokemonChanged::messageJsonForOwner()
{
    return QJsonObject();
}

QJsonObject NotificationDataPokemonChanged::messageJsonForOthers()
{
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifDataPokemonChanged);
    jsonResponse["namePlayer"] = namePlayer();
    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = m_indexCard;
    jsonResponse["lifeLeft"] = m_lifeLeft;

    QJsonArray arrayAttacks;
    for(QMap<unsigned int,bool>::iterator it=m_mapAttacksAvailable.begin();it!=m_mapAttacksAvailable.end();++it)
    {
        QJsonObject objAttack;
        objAttack["index"] = it.key();
        objAttack["available"] = it.value();

        arrayAttacks.append(objAttack);
    }
    jsonResponse["attacks"] = arrayAttacks;

    return jsonResponse;
}
