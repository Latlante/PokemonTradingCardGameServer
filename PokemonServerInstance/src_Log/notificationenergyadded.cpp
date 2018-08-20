#include "notificationenergyadded.h"

NotificationEnergyAdded::NotificationEnergyAdded(const QString &namePlayer, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination, unsigned int idEnergy) :
    AbstractNotification(ConstantesShared::PHASE_NotifEnergyAdded, namePlayer),
    m_packetOrigin(packetOrigin),
    m_packetDestination(packetDestination),
    m_indexCardOrigin(indexCardOrigin),
    m_indexCardDestination(indexCardDestination),
    m_idEnergy(idEnergy)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationEnergyAdded::messageJsonForOwner()
{
    return messageJsonForOthers();
}

QJsonObject NotificationEnergyAdded::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["idPacketOrigin"] = static_cast<int>(m_packetOrigin);
    jsonResponse["indexCardOrigin"] = static_cast<int>(m_indexCardOrigin);
    jsonResponse["idPacketDestination"] = static_cast<int>(m_packetDestination);
    jsonResponse["indexCardDestination"] = static_cast<int>(m_indexCardDestination);
    jsonResponse["elementEnergy"] = static_cast<int>(m_idEnergy);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
