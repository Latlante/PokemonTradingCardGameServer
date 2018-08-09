#include "notificationenergyadded.h"

NotificationEnergyAdded::NotificationEnergyAdded(const QString &namePlayer, ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int idEnergy) :
    AbstractNotification(ConstantesShared::PHASE_NotifEnergyAdded, namePlayer),
    m_packet(packet),
    m_indexCard(indexCard),
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

    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = static_cast<int>(m_indexCard);
    jsonResponse["elementEnergy"] = static_cast<int>(m_idEnergy);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
