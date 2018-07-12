#ifndef FIGHTAREA_H
#define FIGHTAREA_H

#include "abstractpacketstatic.h"

class CardPokemon;

class FightArea : public AbstractPacketStatic
{
    Q_OBJECT

public:
    FightArea(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~FightArea();
	
    int maxCards() const override;

    CardPokemon* pokemonFighter();
    CardPokemon* pokemonFighting(int index);

signals:
    void pokemonFighterChanged();

private slots:
    void onCountChanged();
};

#endif // FIGHTAREA_H
