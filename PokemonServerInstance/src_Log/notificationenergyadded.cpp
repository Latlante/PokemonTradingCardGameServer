#include "notificationenergyadded.h"

NotificationEnergyAdded::NotificationEnergyAdded(ConstantesShared::EnumPacket packet, unsigned int indexCard, unsigned int idEnergy) :
    AbstractNotification(),
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

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifEnergyAdded);
    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = static_cast<int>(m_indexCard);
    jsonResponse["idEnergy"] = static_cast<int>(m_idEnergy);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
