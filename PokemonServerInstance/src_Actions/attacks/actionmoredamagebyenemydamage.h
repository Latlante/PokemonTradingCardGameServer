#ifndef ACTIONMOREDAMAGEBYENEMYDAMAGE_H
#define ACTIONMOREDAMAGEBYENEMYDAMAGE_H

#include "../abstractaction.h"

class ActionMoreDamageByEnemyDamage : public AbstractAction
{
public:
    ActionMoreDamageByEnemyDamage(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionBeforeAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damage;
};

#endif // ACTIONMOREDAMAGEBYENEMYDAMAGE_H
