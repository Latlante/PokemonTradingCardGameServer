#ifndef NOTIFICATIONCARDMOVED_H
#define NOTIFICATIONCARDMOVED_H

#include "abstractnotification.h"

class NotificationCardMoved : public AbstractNotification
{
public:
    NotificationCardMoved(const QString& namePlayer, unsigned int idCard, ConstantesShared::EnumPacket packetOrigin, unsigned int indexCardOrigin, ConstantesShared::EnumPacket packetDestination, unsigned int indexCardDestination);

    virtual QJsonObject messageJsonForOwner() override;
    virtual QJsonObject messageJsonForOthers() override;

private:
    unsigned int m_idCard;
    ConstantesShared::EnumPacket m_packetOrigin, m_packetDestination;
    unsigned int m_indexCardOrigin, m_indexCardDestination;
};

#endif // NOTIFICATIONCARDMOVED_H
