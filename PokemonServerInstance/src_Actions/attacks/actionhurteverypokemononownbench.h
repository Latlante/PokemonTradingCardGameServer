#ifndef ACTIONHURTEVERYPOKEMONONOWNBENCH_H
#define ACTIONHURTEVERYPOKEMONONOWNBENCH_H

#include "../abstractaction.h"

class ActionHurtEveryPokemonOnOwnBench : public AbstractAction
{
public:
    ActionHurtEveryPokemonOnOwnBench(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damage;
};

#endif // ACTIONHURTEVERYPOKEMONONOWNBENCH_H
