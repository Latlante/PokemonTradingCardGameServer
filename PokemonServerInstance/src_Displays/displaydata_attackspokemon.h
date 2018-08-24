#ifndef DISPLAYDATA_ATTACKSPOKEMON_H
#define DISPLAYDATA_ATTACKSPOKEMON_H

#include "abstractdisplaydata.h"

class DisplayData_AttacksPokemon : public AbstractDisplayData
{
    Q_OBJECT
public:
    explicit DisplayData_AttacksPokemon(const QString &namePlayer, int idPokemon, bool retreatEnable);

    virtual QJsonDocument messageInfoToClient() override;
    virtual bool messageResponseFromClient(const QJsonDocument &document) override;
    virtual QJsonObject messageResultToClient() override;

signals:

private:
    int m_idPokemon;
    bool m_retreatEnable;
};

#endif // DISPLAYDATA_ATTACKSPOKEMON_H
