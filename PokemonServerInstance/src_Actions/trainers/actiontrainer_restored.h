#ifndef ACTIONTRAINER_RESTORED_H
#define ACTIONTRAINER_RESTORED_H

#include "../abstractaction.h"

class ActionTrainer_Restored : public AbstractAction
{
public:
    ActionTrainer_Restored();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_RESTORED_H
