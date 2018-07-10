#ifndef ACTIONPROTECTEDAGAINSTDAMAGERANDOM_H
#define ACTIONPROTECTEDAGAINSTDAMAGERANDOM_H

#include "../abstractaction.h"

class ActionProtectedAgainstDamageRandom : public AbstractAction
{
public:
    ActionProtectedAgainstDamageRandom();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONPROTECTEDAGAINSTDAMAGERANDOM_H
