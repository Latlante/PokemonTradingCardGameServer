#ifndef ACTIONTRAINER_SUPERPOTION_H
#define ACTIONTRAINER_SUPERPOTION_H

#include "../abstractaction.h"

class ActionTrainer_SuperPotion : public AbstractAction
{
public:
    ActionTrainer_SuperPotion();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_SUPERPOTION_H
