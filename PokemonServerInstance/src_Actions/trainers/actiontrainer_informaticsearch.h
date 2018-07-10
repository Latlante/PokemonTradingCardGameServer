#ifndef ACTIONTRAINER_INFORMATICSEARCH_H
#define ACTIONTRAINER_INFORMATICSEARCH_H

#include "../abstractaction.h"

class ActionTrainer_InformaticSearch : public AbstractAction
{
public:
    ActionTrainer_InformaticSearch();

    AbstractAction::Enum_typeOfAction type() override;
    QList<AbstractAction::Enum_ElementsToCheck> elementToCheck() override;
    void actionBeforeAttack() override;
};

#endif // ACTIONTRAINER_INFORMATICSEARCH_H
