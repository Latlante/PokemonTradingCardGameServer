#ifndef ACTIONENEMYPOISONED_H
#define ACTIONENEMYPOISONED_H

#include "../abstractaction.h"

class ActionEnemyPoisoned : public AbstractAction
{
public:
    ActionEnemyPoisoned(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_damage;
};

#endif // ACTIONENEMYPOISONED_H
