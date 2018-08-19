#include "notificationpokemonswitched.h"

NotificationPokemonSwitched::NotificationPokemonSwitched(const QString &namePlayer, ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int newIdCard, bool keepEnergy) :
    AbstractNotification(ConstantesShared::PHASE_NotifPokemonSwitched, namePlayer),
    m_packet(packet),
    m_indexCard(indexCard),
    m_newIdCard(newIdCard),
    m_keepEnergy(keepEnergy)
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
    jsonResponse["keepEnergy"] = m_keepEnergy;

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
