#ifndef ACTIONREPLICATELASTDAMAGETOENEMY_H
#define ACTIONREPLICATELASTDAMAGETOENEMY_H

#include "../abstractaction.h"

class ActionReplicateLastDamageToEnemy : public AbstractAction
{
public:
    ActionReplicateLastDamageToEnemy();

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
};

#endif // ACTIONREPLICATELASTDAMAGETOENEMY_H
