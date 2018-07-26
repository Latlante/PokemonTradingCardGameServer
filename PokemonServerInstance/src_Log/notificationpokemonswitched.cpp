#include "notificationpokemonswitched.h"

NotificationPokemonSwitched::NotificationPokemonSwitched(const QString &namePlayer, ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int newIdCard) :
    AbstractNotification(ConstantesShared::PHASE_NotifPokemonSwitched, namePlayer),
    m_packet(packet),
    m_indexCard(indexCard),
    m_newIdCard(newIdCard)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationPokemonSwitched::messageJsonForOwner()
{
    return messageJsonForOthers();
}

QJsonObject NotificationPokemonSwitched::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = static_cast<int>(m_indexCard);
    jsonResponse["newIdCard"] = static_cast<int>(m_newIdCard);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
