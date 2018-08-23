#ifndef DISPLAYDATA_PACKET_H
#define DISPLAYDATA_PACKET_H

#include "abstractdisplaydata.h"
#include "src_Cards/abstractcard.h"

class AbstractPacket;

class DisplayData_Packet : public AbstractDisplayData
{
    Q_OBJECT
public:
    DisplayData_Packet(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard);

    virtual QJsonDocument messageInfoToClient() override;
    virtual bool messageResponseFromClient(const QJsonDocument &document) override;
    virtual QJsonObject messageResultToClient() override;

private:
    AbstractPacket* m_packet;
    unsigned short m_quantity;
    AbstractCard::Enum_typeOfCard m_typeOfCard;
};

#endif // DISPLAYDATA_PACKET_H
