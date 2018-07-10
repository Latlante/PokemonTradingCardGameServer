#ifndef ACTIONMOREDAMAGEORHURTHIMSELF_H
#define ACTIONMOREDAMAGEORHURTHIMSELF_H

#include "../abstractaction.h"

class ActionMoreDamageOrHurtHimSelf : public AbstractAction
{
public:
    ActionMoreDamageOrHurtHimSelf(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damageOnHead;
    unsigned short m_damageOnTail;
};

#endif // ACTIONMOREDAMAGEORHURTHIMSELF_H
