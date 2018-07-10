#ifndef ACTIONREPLICATEONEATTACKFROMENEMY_H
#define ACTIONREPLICATEONEATTACKFROMENEMY_H

#include "../abstractaction.h"

class ActionReplicateOneAttackFromEnemy : public AbstractAction
{
public:
    ActionReplicateOneAttackFromEnemy();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONREPLICATEONEATTACKFROMENEMY_H
