#ifndef DISPLAYDATA_ENERGIESFORAPOKEMON_H
#define DISPLAYDATA_ENERGIESFORAPOKEMON_H

#include "abstractdisplaydata.h"

#include "src_Cards/abstractcard.h"

class CardPokemon;

class DisplayData_EnergiesForAPokemon : public AbstractDisplayData
{
    Q_OBJECT
public:
    explicit DisplayData_EnergiesForAPokemon(const QString &namePlayer, CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element);

    virtual QJsonDocument messageInfoToClient() override;
    virtual bool messageResponseFromClient(const QJsonDocument &document) override;
    virtual QJsonObject messageResultToClient() override;

signals:

private:
    CardPokemon* m_pokemon;
    unsigned short m_quantity;
    AbstractCard::Enum_element m_element;
};

#endif // DISPLAYDATA_ENERGIESFORAPOKEMON_H
