#ifndef ACTIONDIEANDHURTEVERYPOKEMONONEACHBENCH_H
#define ACTIONDIEANDHURTEVERYPOKEMONONEACHBENCH_H

#include "../abstractaction.h"

class ActionDieAndHurtEveryPokemonOnEachBench : public AbstractAction
{
public:
    ActionDieAndHurtEveryPokemonOnEachBench(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damage;
};

#endif // ACTIONDIEANDHURTEVERYPOKEMONONEACHBENCH_H
