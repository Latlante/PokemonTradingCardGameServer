#ifndef ACTIONMOREDAMAGEBYOWNDAMAGE_H
#define ACTIONMOREDAMAGEBYOWNDAMAGE_H

#include "../abstractaction.h"

class ActionMoreDamageByOwnDamage : public AbstractAction
{
public:
    ActionMoreDamageByOwnDamage(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damage;
};

#endif // ACTIONMOREDAMAGEBYOWNDAMAGE_H
