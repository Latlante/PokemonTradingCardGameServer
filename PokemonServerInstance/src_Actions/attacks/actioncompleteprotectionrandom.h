#ifndef ACTIONCOMPLETEPROTECTIONRANDOM_H
#define ACTIONCOMPLETEPROTECTIONRANDOM_H

#include "../abstractaction.h"

class ActionCompleteProtectionRandom : public AbstractAction
{
public:
    ActionCompleteProtectionRandom();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONCOMPLETEPROTECTIONRANDOM_H
