#include "notificationenergyremoved.h"

NotificationEnergyRemoved::NotificationEnergyRemoved(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, unsigned int indexEnergy) :
    AbstractNotification(ConstantesShared::PHASE_NotifEnergyRemoved, namePlayer),
    m_packetOrigin(packetOrigin),
    m_packetDestination(packetDestination),
    m_indexCardOrigin(indexCardOrigin),
    m_indexCardDestination(indexCardDestination),
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

    jsonResponse["idPacket"] = static_cast<int>(m_packetOrigin);
    jsonResponse["indexCard"] = static_cast<int>(m_indexCardOrigin);
    jsonResponse["indexEnergy"] = static_cast<int>(m_indexEnergy);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
