#ifndef ACTIONUNIQUEATTACK_RANDOM_H
#define ACTIONUNIQUEATTACK_RANDOM_H

#include "../abstractaction.h"

class ActionUniqueAttack_Random : public AbstractAction
{
public:
    ActionUniqueAttack_Random();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONUNIQUEATTACK_RANDOM_H
