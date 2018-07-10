#ifndef ACTIONCHANGEWEAKNESSOFENEMY_H
#define ACTIONCHANGEWEAKNESSOFENEMY_H

#include "../abstractaction.h"
#include "src_Cards/abstractcard.h"

class ActionChangeWeaknessOfEnemy : public AbstractAction
{
public:
    ActionChangeWeaknessOfEnemy();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

};

#endif // ACTIONCHANGEWEAKNESSOFENEMY_H
