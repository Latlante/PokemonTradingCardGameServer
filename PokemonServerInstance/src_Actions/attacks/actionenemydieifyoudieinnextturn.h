#ifndef ACTIONENEMYDIEIFYOUDIEINNEXTTURN_H
#define ACTIONENEMYDIEIFYOUDIEINNEXTTURN_H

#include "../abstractaction.h"

class ActionEnemyDieIfYouDieInNextTurn : public AbstractAction
{
public:
    ActionEnemyDieIfYouDieInNextTurn();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

};

#endif // ACTIONENEMYDIEIFYOUDIEINNEXTTURN_H
