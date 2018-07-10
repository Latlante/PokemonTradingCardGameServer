#ifndef ACTIONATTACKONLYIFENEMYISSLEEPING_H
#define ACTIONATTACKONLYIFENEMYISSLEEPING_H

#include "../abstractaction.h"
#include "src_Cards/abstractcard.h"

class ActionAttackOnlyIfEnemyIsSleeping : public AbstractAction
{
public:
    ActionAttackOnlyIfEnemyIsSleeping(QVariant arg1);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damage;
};

#endif // ACTIONATTACKONLYIFENEMYISSLEEPING_H
