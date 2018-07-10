#ifndef ACTIONTRAINER_POTION_H
#define ACTIONTRAINER_POTION_H

#include "../abstractaction.h"

class ActionTrainer_Potion : public AbstractAction
{
public:
    ActionTrainer_Potion();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_POTION_H
