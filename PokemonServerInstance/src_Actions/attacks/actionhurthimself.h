#ifndef ACTIONHURTHIMSELF_H
#define ACTIONHURTHIMSELF_H

#include "../abstractaction.h"

class ActionHurtHimself : public AbstractAction
{
public:
    ActionHurtHimself(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck();

private:
    unsigned short m_damage;
};

#endif // ACTIONHURTHIMSELF_H
