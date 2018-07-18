#include "notificationcardmoved.h"

NotificationCardMoved::NotificationCardMoved(const QString &namePlayer, unsigned int idCard, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination) :
    AbstractNotification(namePlayer),
    m_idCard(idCard),
    m_packetOrigin(packetOrigin),
    m_packetDestination(packetDestination),
    m_indexCardOrigin(indexCardOrigin),
    m_indexCardDestination(indexCardDestination)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationCardMoved::messageJsonForOwner()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifCardMoved);
    jsonResponse["idCard"] = static_cast<int>(m_idCard);
    jsonResponse["idPacketOrigin"] = static_cast<int>(m_packetOrigin);
    jsonResponse["indexCardOrigin"] = static_cast<int>(m_indexCardOrigin);
    jsonResponse["idPacketDestination"] = static_cast<int>(m_packetDestination);
    jsonResponse["indexCardDestination"] = static_cast<int>(m_indexCardDestination);

    return jsonResponse;
}

QJsonObject NotificationCardMoved::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    jsonResponse["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifCardMoved);
    jsonResponse["idPacketOrigin"] = static_cast<int>(m_packetOrigin);
    jsonResponse["indexCardOrigin"] = static_cast<int>(m_indexCardOrigin);
    jsonResponse["idPacketDestination"] = static_cast<int>(m_packetDestination);
    jsonResponse["indexCardDestination"] = static_cast<int>(m_indexCardDestination);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
