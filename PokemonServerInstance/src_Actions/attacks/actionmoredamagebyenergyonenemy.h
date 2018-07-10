#ifndef ACTIONMOREDAMAGEBYENERGYONENEMY_H
#define ACTIONMOREDAMAGEBYENERGYONENEMY_H

#include "../abstractaction.h"

class ActionMoreDamageByEnergyOnEnemy : public AbstractAction
{
public:
    ActionMoreDamageByEnergyOnEnemy(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damageByEnergy;
};

#endif // ACTIONMOREDAMAGEBYENERGYONENEMY_H
