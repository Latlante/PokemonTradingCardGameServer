#include "notificationcardmoved.h"

NotificationCardMoved::NotificationCardMoved(const QString &namePlayer, int idCard, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, bool showCardToEveryone) :
    AbstractNotification(ConstantesShared::PHASE_NotifCardMoved, namePlayer),
    m_idCard(idCard),
    m_packetOrigin(packetOrigin),
    m_packetDestination(packetDestination),
    m_indexCardOrigin(indexCardOrigin),
    m_showCardToEveryone(showCardToEveryone)
{

}

NotificationCardMoved::~NotificationCardMoved()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationCardMoved::messageJsonForOwner()
{
    QJsonObject jsonResponse = messageJsonForOthers();

    if(m_idCard != -1)
        jsonResponse["idCard"] = static_cast<int>(m_idCard);

    jsonResponse["idPacketOrigin"] = static_cast<int>(m_packetOrigin);
    jsonResponse["indexCardOrigin"] = static_cast<int>(m_indexCardOrigin);
    jsonResponse["idPacketDestination"] = static_cast<int>(m_packetDestination);

    return jsonResponse;
}

QJsonObject NotificationCardMoved::messageJsonForOthers()
{
    QJsonObject jsonResponse = initObject();

    if(m_showCardToEveryone == true)
        jsonResponse["idCard"] = static_cast<int>(m_idCard);

    jsonResponse["idPacketOrigin"] = static_cast<int>(m_packetOrigin);
    jsonResponse["indexCardOrigin"] = static_cast<int>(m_indexCardOrigin);
    jsonResponse["idPacketDestination"] = static_cast<int>(m_packetDestination);

    return jsonResponse;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
