#ifndef ACTIONTRAINER_POKEMONCENTER_H
#define ACTIONTRAINER_POKEMONCENTER_H

#include "../abstractaction.h"

class ActionTrainer_PokemonCenter : public AbstractAction
{
public:
    ActionTrainer_PokemonCenter();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_POKEMONCENTER_H
