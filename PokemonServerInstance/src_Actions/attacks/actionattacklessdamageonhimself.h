#ifndef ACTIONATTACKLESSDAMAGEONHIMSELF_H
#define ACTIONATTACKLESSDAMAGEONHIMSELF_H

#include "../abstractaction.h"

class ActionAttackLessDamageOnHimself : public AbstractAction
{
public:
    ActionAttackLessDamageOnHimself(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_originalDamage;
    unsigned short m_damagePerMarquer;
};

#endif // ACTIONATTACKLESSDAMAGEONHIMSELF_H
