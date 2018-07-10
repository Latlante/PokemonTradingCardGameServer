#ifndef ACTIONCHANGEENEMYSTATUSRANDOM_H
#define ACTIONCHANGEENEMYSTATUSRANDOM_H

#include "../abstractaction.h"

class ActionChangeEnemyStatusRandom : public AbstractAction
{
public:
    explicit ActionChangeEnemyStatusRandom(QVariant arg1, QVariant arg2);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    CardPokemon::Enum_statusOfPokemon m_statusOnHead;
    CardPokemon::Enum_statusOfPokemon m_statusOnTail;
};

#endif // ACTIONCHANGEENEMYSTATUSRANDOM_H
