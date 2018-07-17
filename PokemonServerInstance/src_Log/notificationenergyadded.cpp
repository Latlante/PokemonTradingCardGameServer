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

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
QJsonObject NotificationEnergyAdded::messageJsonForOwner()
{
    return QJsonObject();
}

QJsonObject NotificationEnergyAdded::messageJsonForOthers()
{
    QJsonObject jsonResponse;

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifEnergyAdded);
    jsonResponse["idPacket"] = static_cast<int>(m_packet);
    jsonResponse["indexCard"] = m_indexCard;
    jsonResponse["idEnergy"] = m_idEnergy;

    return jsonResponse;
}
