#ifndef ACTIONTRAINER_TOTALGUERISON_H
#define ACTIONTRAINER_TOTALGUERISON_H

#include "../abstractaction.h"

class ActionTrainer_TotalGuerison : public AbstractAction
{
public:
    ActionTrainer_TotalGuerison();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_TOTALGUERISON_H
