#ifndef ACTIONREMOVEENERGYATTACHED_H
#define ACTIONREMOVEENERGYATTACHED_H

#include "../abstractaction.h"

class ActionRemoveEnergyAttached : public AbstractAction
{
public:
    ActionRemoveEnergyAttached(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionBeforeAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_numberOfEnergiesToRemoved;
    AbstractCard::Enum_element m_elementToRemove;
};

#endif // ACTIONREMOVEENERGYATTACHED_H
