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

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject NotificationEnergyRemoved::messageJsonForOwner()
{
    return QJsonObject();
}

QJsonObject NotificationEnergyRemoved::messageJsonForOthers()
{
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifEnergyRemoved);
    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = m_indexCard;
    jsonResponse["indexEnergy"] = m_indexEnergy;

    return jsonResponse;
}
