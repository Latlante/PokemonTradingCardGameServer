#include "notificationpokemonswitched.h"

NotificationPokemonSwitched::NotificationPokemonSwitched(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int newIdCard) :
    AbstractNotification(),
    m_packet(packet),
    m_indexCard(indexCard),
    m_newIdCard(newIdCard)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject NotificationPokemonSwitched::messageJsonForOwner()
{
    return QJsonObject();
}

QJsonObject NotificationPokemonSwitched::messageJsonForOthers()
{
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifPokemonSwitched);
    jsonResponse["namePlayer"] = namePlayer();
    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = m_indexCard;
    jsonResponse["newIdCard"] = m_newIdCard;

    return jsonResponse;
}
