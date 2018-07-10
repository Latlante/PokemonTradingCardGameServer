#ifndef ACTIONSWAPPOKEMONBETWEENFIGTHANDBENCH_H
#define ACTIONSWAPPOKEMONBETWEENFIGTHANDBENCH_H

#include "../abstractaction.h"

class ActionSwapPokemonBetweenFigthAndBench : public AbstractAction
{
public:
    ActionSwapPokemonBetweenFigthAndBench();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONSWAPPOKEMONBETWEENFIGTHANDBENCH_H
