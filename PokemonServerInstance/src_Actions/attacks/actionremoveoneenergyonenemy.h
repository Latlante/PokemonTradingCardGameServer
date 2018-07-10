#ifndef ACTIONREMOVEONEENERGYONENEMY_H
#define ACTIONREMOVEONEENERGYONENEMY_H

#include "../abstractaction.h"

class ActionRemoveOneEnergyOnEnemy : public AbstractAction
{
public:
    ActionRemoveOneEnergyOnEnemy(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_numberOfEnergiesToRemoved;
    AbstractCard::Enum_element m_elementToRemove;
};

#endif // ACTIONREMOVEONEENERGYONENEMY_H
