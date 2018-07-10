#ifndef ACTIONTRAINER_SAVEENERGIES_H
#define ACTIONTRAINER_SAVEENERGIES_H

#include "../abstractaction.h"

class ActionTrainer_SaveEnergies : public AbstractAction
{
public:
    ActionTrainer_SaveEnergies();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_SAVEENERGIES_H
