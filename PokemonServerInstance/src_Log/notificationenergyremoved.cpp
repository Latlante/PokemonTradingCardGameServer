#include "notificationenergyremoved.h"

NotificationEnergyRemoved::NotificationEnergyRemoved(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int indexEnergy) :
    AbstractNotification(),
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

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifEnergyRemoved);
    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = static_cast<int>(m_indexCard);
    jsonResponse["indexEnergy"] = static_cast<int>(m_indexEnergy);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
