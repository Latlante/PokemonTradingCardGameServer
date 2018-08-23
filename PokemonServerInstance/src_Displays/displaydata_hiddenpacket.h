#ifndef DISPLAYDATA_HIDDENPACKET_H
#define DISPLAYDATA_HIDDENPACKET_H

#include "abstractdisplaydata.h"

class AbstractPacket;

class DisplayData_HiddenPacket : public AbstractDisplayData
{
    Q_OBJECT
public:
    explicit DisplayData_HiddenPacket(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity);

    virtual QJsonDocument messageInfoToClient() override;
    virtual bool messageResponseFromClient(const QJsonDocument &document) override;
    virtual QJsonObject messageResultToClient() override;

signals:

private:
    AbstractPacket* m_packet;
    unsigned short m_quantity;
};

#endif // DISPLAYDATA_HIDDENPACKET_H
