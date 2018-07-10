#ifndef ACTIONTRAINER_FAKEPROFESSORCHEN_H
#define ACTIONTRAINER_FAKEPROFESSORCHEN_H

#include "../abstractaction.h"

class ActionTrainer_FakeProfessorChen : public AbstractAction
{
public:
    ActionTrainer_FakeProfessorChen();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_FAKEPROFESSORCHEN_H
