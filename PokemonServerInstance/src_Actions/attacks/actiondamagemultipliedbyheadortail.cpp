#include "actiondamagemultipliedbyheadortail.h"


ActionDamageMultipliedByHeadOrTail::ActionDamageMultipliedByHeadOrTail(QVariant arg1, QVariant arg2) :
    AbstractAction(arg1, arg2)
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionDamageMultipliedByHeadOrTail::type()
{
    return AbstractAction::Action_DamageMultipliedByHeadOrTail;
}

bool ActionDamageMultipliedByHeadOrTail::checkElements()
{
    bool ok;

    m_damageByHead = m_arg1.toInt(&ok);

    if(ok)
        m_numberOfCoins = m_arg2.toInt(&ok);

    return ok;
}

void ActionDamageMultipliedByHeadOrTail::actionAfterAttack()
{
    if(pokemonAttacked() != nullptr)
    {
        unsigned short totalDamage = 0;

        for(int i=0;i<m_numberOfCoins;++i)
        {
            unsigned short coin = headOrTail();
            if(coin == 1)
                totalDamage += m_damageByHead;
        }

        pokemonAttacked()->takeDamage(totalDamage);
    }
}

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
QList<AbstractAction::Enum_ElementsToCheck> ActionDamageMultipliedByHeadOrTail::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckPokemonAttacked;
}
