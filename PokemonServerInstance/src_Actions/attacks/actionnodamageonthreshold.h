#ifndef ACTIONNODAMAGEONTHRESHOLD_H
#define ACTIONNODAMAGEONTHRESHOLD_H

#include "../abstractaction.h"

class ActionNoDamageOnThreshold : public AbstractAction
{
public:
    ActionNoDamageOnThreshold(QVariant arg);

    AbstractAction::Enum_typeOfAction type() override;
    bool checkElements() override;
    void actionAfterAttack() override;

protected:
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;

private:
    unsigned short m_threshold;
};

#endif // ACTIONNODAMAGEONTHRESHOLD_H
