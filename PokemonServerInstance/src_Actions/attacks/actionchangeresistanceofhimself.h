#ifndef ACTIONCHANGERESISTANCEOFHIMSELF_H
#define ACTIONCHANGERESISTANCEOFHIMSELF_H

#include "../abstractaction.h"
#include "src_Cards/abstractcard.h"

class ActionChangeResistanceOfHimself : public AbstractAction
{
public:
    ActionChangeResistanceOfHimself();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

};

#endif // ACTIONCHANGERESISTANCEOFHIMSELF_H
