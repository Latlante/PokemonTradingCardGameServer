#ifndef ACTIONHEALING_H
#define ACTIONHEALING_H

#include "../abstractaction.h"

class ActionHealing : public AbstractAction
{
public:
    ActionHealing(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionBeforeAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_pv;
    AbstractCard::Enum_element m_energyToPay;
};

#endif // ACTIONHEALING_H
