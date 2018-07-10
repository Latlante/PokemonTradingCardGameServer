#ifndef ACTIONTRAINER_PROFESSORCHEN_H
#define ACTIONTRAINER_PROFESSORCHEN_H

#include "../abstractaction.h"

class ActionTrainer_ProfessorChen : public AbstractAction
{
public:
    ActionTrainer_ProfessorChen();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_PROFESSORCHEN_H
