#ifndef ACTIONDAMAGEOFHALFENEMYLIFELEFT_H
#define ACTIONDAMAGEOFHALFENEMYLIFELEFT_H

#include "../abstractaction.h"

class ActionDamageOfHalfEnemyLifeLeft : public AbstractAction
{
public:
    ActionDamageOfHalfEnemyLifeLeft();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONDAMAGEOFHALFENEMYLIFELEFT_H
