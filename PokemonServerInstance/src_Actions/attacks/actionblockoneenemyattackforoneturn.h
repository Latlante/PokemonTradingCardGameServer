#ifndef ACTIONBLOCKONEENEMYATTACKFORONETURN_H
#define ACTIONBLOCKONEENEMYATTACKFORONETURN_H

#include "../abstractaction.h"

class ActionBlockOneEnemyAttackForOneTurn : public AbstractAction
{
public:
    ActionBlockOneEnemyAttackForOneTurn();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONBLOCKONEENEMYATTACKFORONETURN_H
