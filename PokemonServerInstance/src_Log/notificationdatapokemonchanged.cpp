#include "notificationdatapokemonchanged.h"

NotificationDataPokemonChanged::NotificationDataPokemonChanged(const QString &namePlayer, ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int lifeLeft, QMap<unsigned int, bool> mapAttacksAvailable, QList<unsigned int> listIdEnergies) :
    AbstractNotification(ConstantesShared::PHASE_NotifDataPokemonChanged, namePlayer),
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
QJsonObject NotificationDataPokemonChanged::messageJsonForOwner()
{
    return messageJsonForOthers();
}

QJsonObject NotificationDataPokemonChanged::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = static_cast<int>(m_indexCard);
    jsonResponse["lifeLeft"] = static_cast<int>(m_lifeLeft);

    QJsonArray arrayAttacks;
    for(QMap<unsigned int,bool>::iterator it=m_mapAttacksAvailable.begin();it!=m_mapAttacksAvailable.end();++it)
    {
        QJsonObject objAttack;
        objAttack["index"] = static_cast<int>(it.key());
        objAttack["available"] = it.value();

        arrayAttacks.append(objAttack);
    }
    jsonResponse["attacks"] = arrayAttacks;

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
