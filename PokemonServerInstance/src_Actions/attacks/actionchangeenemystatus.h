#ifndef ACTIONCHANGEENEMYSTATUS_H
#define ACTIONCHANGEENEMYSTATUS_H

#include "../abstractaction.h"

class ActionChangeEnemyStatus : public AbstractAction
{
public:
    explicit ActionChangeEnemyStatus(QVariant arg1);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    CardPokemon::Enum_statusOfPokemon m_status;
};

#endif // ACTIONCHANGEENEMYSTATUS_H
