#ifndef ACTIONDAMAGEMULTIPLIEDBYHEADORTAIL_H
#define ACTIONDAMAGEMULTIPLIEDBYHEADORTAIL_H

#include "../abstractaction.h"

class ActionDamageMultipliedByHeadOrTail : public AbstractAction
{
public:
    ActionDamageMultipliedByHeadOrTail(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damageByHead;
    unsigned short m_numberOfCoins;
};

#endif // ACTIONDAMAGEMULTIPLIEDBYHEADORTAIL_H
