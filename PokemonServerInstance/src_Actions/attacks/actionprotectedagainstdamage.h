#ifndef ACTIONPROTECTEDAGAINSTDAMAGE_H
#define ACTIONPROTECTEDAGAINSTDAMAGE_H

#include "../abstractaction.h"

class ActionProtectedAgainstDamage : public AbstractAction
{
public:
    ActionProtectedAgainstDamage();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONPROTECTEDAGAINSTDAMAGE_H
