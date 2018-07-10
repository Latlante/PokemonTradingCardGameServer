#ifndef ACTIONTRAINER_LITTLEGIRL_H
#define ACTIONTRAINER_LITTLEGIRL_H

#include "../abstractaction.h"

class ActionTrainer_LittleGirl : public AbstractAction
{
public:
    ActionTrainer_LittleGirl();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_LITTLEGIRL_H
