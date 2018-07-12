#ifndef BENCHAREA_H
#define BENCHAREA_H

#include "abstractpacketstatic.h"

class CardPokemon;
class ModelListEnergies;

class BenchArea : public AbstractPacketStatic
{
    Q_OBJECT

public:
    BenchArea(const QString& namePacket, QList<AbstractCard*> listCards = QList<AbstractCard*>());
    virtual ~BenchArea();
	
    int maxCards() const override;
    CardPokemon* cardPok(int index);

    ModelListEnergies* modelFromCardPokemon(int index);

private:

};

#endif // BENCHAREA_H
