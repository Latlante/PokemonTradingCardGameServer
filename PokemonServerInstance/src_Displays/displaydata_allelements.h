#ifndef DISPLAYDATA_ALLELEMENTS_H
#define DISPLAYDATA_ALLELEMENTS_H

#include "abstractdisplaydata.h"

class DisplayData_AllElements : public AbstractDisplayData
{
    Q_OBJECT
public:
    explicit DisplayData_AllElements(const QString &namePlayer, unsigned short quantity);

    virtual QJsonDocument messageInfoToClient() override;
    virtual bool messageResponseFromClient(const QJsonDocument &document) override;
    virtual QJsonObject messageResultToClient() override;

signals:

private:
    unsigned short m_quantity;
};

#endif // DISPLAYDATA_ALLELEMENTS_H
