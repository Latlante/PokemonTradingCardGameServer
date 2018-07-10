#include "actiontrainer_pokemoncenter.h"

ActionTrainer_PokemonCenter::ActionTrainer_PokemonCenter() :
    AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
AbstractAction::Enum_typeOfAction ActionTrainer_PokemonCenter::type()
{
    return AbstractAction::ActionTrainer_PokemonCenter;
}

QList<AbstractAction::Enum_ElementsToCheck> ActionTrainer_PokemonCenter::elementToCheck()
{
    return QList<AbstractAction::Enum_ElementsToCheck>()
            << AbstractAction::CheckBenchPlayerAttacking
            << AbstractAction::CheckPokemonAttacking;
}

void ActionTrainer_PokemonCenter::actionBeforeAttack()
{
    if((benchPlayerAttacking() != nullptr) && (pokemonAttacking() != nullptr))
    {
        //Pour le pokémon sur la zone de combat
        //On restaure les PV
        pokemonAttacking()->restoreLife(pokemonAttacking()->lifeTotal());
        //On retire toutes les énergies
        pokemonAttacking()->moveAllEnergiesInTrash();

        //Pour chaque pokémon sur le banc
        for(int i=0;i<benchPlayerAttacking()->countCard();++i)
        {
            CardPokemon* pokemonBench = benchPlayerAttacking()->cardPok(i);

            //On restaure les PV
            pokemonBench->restoreLife(pokemonBench->lifeTotal());

            //On retire toutes les énergies
            pokemonBench->moveAllEnergiesInTrash();
        }
    }
}
