#ifndef ACTIONENEMYCANATTACKONNEXTTURN_RANDOM_H
#define ACTIONENEMYCANATTACKONNEXTTURN_RANDOM_H

#include "../abstractaction.h"

class ActionEnemyCanAttackOnNextTurn_Random : public AbstractAction
{
public:
    ActionEnemyCanAttackOnNextTurn_Random();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONENEMYCANATTACKONNEXTTURN_RANDOM_H
