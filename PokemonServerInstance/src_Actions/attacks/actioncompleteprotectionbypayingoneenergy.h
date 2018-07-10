#ifndef ACTIONCOMPLETEPROTECTIONBYPAYINGONEENERGY_H
#define ACTIONCOMPLETEPROTECTIONBYPAYINGONEENERGY_H

#include "../abstractaction.h"

class ActionCompleteProtectionByPayingOneEnergy : public AbstractAction
{
public:
    ActionCompleteProtectionByPayingOneEnergy(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionBeforeAttack();

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    AbstractCard::Enum_element m_element;
};

#endif // ACTIONCOMPLETEPROTECTIONBYPAYINGONEENERGY_H
