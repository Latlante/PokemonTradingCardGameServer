#ifndef ACTIONMOREDAMAGEBYENERGY_H
#define ACTIONMOREDAMAGEBYENERGY_H

#include "../abstractaction.h"

class ActionMoreDamageByEnergy : public AbstractAction
{
public:
    ActionMoreDamageByEnergy(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damageByEnergy;
    unsigned short m_indexOfAttack;

};

#endif // ACTIONMOREDAMAGEBYENERGY_H
