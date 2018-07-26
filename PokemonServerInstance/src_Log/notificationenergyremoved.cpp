#include "notificationenergyremoved.h"

NotificationEnergyRemoved::NotificationEnergyRemoved(const QString &namePlayer, ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int indexEnergy) :
    AbstractNotification(ConstantesShared::PHASE_NotifEnergyRemoved, namePlayer),
    m_packet(packet),
    m_indexCard(indexCard),
    m_indexEnergy(indexEnergy)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationEnergyRemoved::messageJsonForOwner()
{
    return messageJsonForOthers();
}

QJsonObject NotificationEnergyRemoved::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = static_cast<int>(m_indexCard);
    jsonResponse["indexEnergy"] = static_cast<int>(m_indexEnergy);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
