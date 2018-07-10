#ifndef ACTIONTRAINER_REMOVEENERGY_H
#define ACTIONTRAINER_REMOVEENERGY_H

#include "../abstractaction.h"

class ActionTrainer_RemoveEnergy : public AbstractAction
{
public:
    ActionTrainer_RemoveEnergy();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_REMOVEENERGY_H
