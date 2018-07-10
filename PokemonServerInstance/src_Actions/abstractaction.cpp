#include "abstractaction.h"

AbstractAction::AbstractAction() :
    m_arg1(QVariant()),
    m_arg2(QVariant()),
    m_gameManager(nullptr),
    m_playerAttacked(nullptr),
    m_playerAttacking(nullptr),
    m_pokemonAttached(nullptr),
    m_pokemonAttacked(nullptr),
    m_pokemonAttacking(nullptr),
    m_benchPlayerAttacked(nullptr),
    m_benchPlayerAttacking(nullptr),
    m_indexAttack(-1)
#ifdef TESTS_UNITAIRES
    , m_listCoins(QList<unsigned short>())
#endif
{
}

AbstractAction::AbstractAction(QVariant arg1) :
    m_arg1(arg1),
    m_arg2(QVariant()),
    m_gameManager(nullptr),
    m_playerAttacked(nullptr),
    m_playerAttacking(nullptr),
    m_pokemonAttached(nullptr),
    m_pokemonAttacked(nullptr),
    m_pokemonAttacking(nullptr),
    m_benchPlayerAttacked(nullptr),
    m_benchPlayerAttacking(nullptr),
    m_indexAttack(-1)
#ifdef TESTS_UNITAIRES
    , m_listCoins(QList<unsigned short>())
#endif
{
}


AbstractAction::AbstractAction(QVariant arg1, QVariant arg2) :
    m_arg1(arg1),
    m_arg2(arg2),
    m_gameManager(nullptr),
    m_playerAttacked(nullptr),
    m_playerAttacking(nullptr),
    m_pokemonAttached(nullptr),
    m_pokemonAttacked(nullptr),
    m_pokemonAttacking(nullptr),
    m_benchPlayerAttacked(nullptr),
    m_benchPlayerAttacking(nullptr),
    m_indexAttack(-1)
#ifdef TESTS_UNITAIRES
    , m_listCoins(QList<unsigned short>())
#endif
{
}

AbstractAction::~AbstractAction()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QVariant AbstractAction::arg1()
{
    return m_arg1;
}

QVariant AbstractAction::arg2()
{
    return m_arg2;
}

void AbstractAction::executeActionBeforeAttack(CardPokemon *pokemonAttached, short index)
{
    m_pokemonAttached = pokemonAttached;

    //Vérifications
    QList<AbstractAction::Enum_ElementsToCheck> listElementsToCheck = elementToCheck();
    bool statusCanBeExecuted = true;
    m_indexAttack = index;

    foreach(AbstractAction::Enum_ElementsToCheck element, listElementsToCheck)
    {
        switch(element)
        {
        case AbstractAction::CheckGameManager:
            statusCanBeExecuted &= checkGameManager();
            break;
        case AbstractAction::CheckPlayerAttacked:
            statusCanBeExecuted &= checkPlayerAttacked();
            break;
        case AbstractAction::CheckPlayerAttacking:
            statusCanBeExecuted &= checkPlayerAttacking();
            break;
        case AbstractAction::CheckPokemonAttached:
            statusCanBeExecuted &= checkPokemonAttached();
            break;
        case AbstractAction::CheckPokemonAttacked:
            statusCanBeExecuted &= checkPokemonAttacked();
            break;
        case AbstractAction::CheckPokemonAttacking:
            statusCanBeExecuted &= checkPokemonAttacking();
            break;
        case AbstractAction::CheckBenchPlayerAttacked:
            statusCanBeExecuted &= checkBenchPlayerAttacked();
            break;
        case AbstractAction::CheckBenchPlayerAttacking:
            statusCanBeExecuted &= checkBenchPlayerAttacking();
            break;
        default:
            qCritical() << __PRETTY_FUNCTION__ << "Erreur elementToCheck" << element;
            statusCanBeExecuted = false;
        }
    }

    //Execution
    if(statusCanBeExecuted == true)
    {
        actionBeforeAttack();
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "L'action n'a pas pu être executée";
    }
}

void AbstractAction::executeActionAfterAttack(CardPokemon *pokemonAttached, short index)
{
    m_pokemonAttached = pokemonAttached;

    //Vérifications
    //QList<AbstractAction::Enum_ElementsToCheck> listElementsToCheck = elementToCheck();
    bool statusCanBeExecuted = true;
    m_indexAttack = index;

    /*foreach(AbstractAction::Enum_ElementsToCheck element, listElementsToCheck)
    {
        switch(element)
        {
        case AbstractAction::CheckGameManager:
            statusCanBeExecuted &= checkGameManager();
            break;
        case AbstractAction::CheckPlayerAttacked:
            statusCanBeExecuted &= checkPlayerAttacked();
            break;
        case AbstractAction::CheckPlayerAttacking:
            statusCanBeExecuted &= checkPlayerAttacking();
            break;
        case AbstractAction::CheckPokemonAttached:
            statusCanBeExecuted &= checkPokemonAttached();
            break;
        case AbstractAction::CheckPokemonAttacked:
            statusCanBeExecuted &= checkPokemonAttacked();
            break;
        case AbstractAction::CheckPokemonAttacking:
            statusCanBeExecuted &= checkPokemonAttacking();
            break;
        case AbstractAction::CheckBenchPlayerAttacked:
            statusCanBeExecuted &= checkBenchPlayerAttacked();
            break;
        case AbstractAction::CheckBenchPlayerAttacking:
            statusCanBeExecuted &= checkBenchPlayerAttacking();
            break;
        default:
            qCritical() << __PRETTY_FUNCTION__ << "Erreur elementToCheck" << element;
            statusCanBeExecuted = false;
        }
    }*/

    //Execution
    if(statusCanBeExecuted == true)
    {
        actionAfterAttack();
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << "L'action n'a pas pu être executée";
    }
}

#ifdef TESTS_UNITAIRES
QList<unsigned short> AbstractAction::listCoins()
{
    return m_listCoins;
}
#endif

/************************************************************
*****				FONCTIONS PROTEGEES					*****
************************************************************/
void AbstractAction::actionBeforeAttack()
{

}

void AbstractAction::actionAfterAttack()
{

}

//Pourra être utile plus tard pour les tests
unsigned short AbstractAction::headOrTail()
{
    unsigned short coin = 0;

    if(gameManager() != nullptr)
        coin = gameManager()->headOrTail();

#ifdef TESTS_UNITAIRES
    m_listCoins.append(coin);
    qDebug() << "Coin = " << coin;
#endif

    return coin;
}

GameManager* AbstractAction::gameManager()
{
    return m_gameManager;
}

Player* AbstractAction::playerAttacked()
{
    return m_playerAttacked;
}

Player* AbstractAction::playerAttacking()
{
    return m_playerAttacking;
}

CardPokemon* AbstractAction::pokemonAttached()
{
    return m_pokemonAttached;
}

CardPokemon* AbstractAction::pokemonAttacked()
{
    return m_pokemonAttacked;
}

CardPokemon* AbstractAction::pokemonAttacking()
{
    return m_pokemonAttacking;
}

BenchArea* AbstractAction::benchPlayerAttacked()
{
    return m_benchPlayerAttacked;
}

BenchArea* AbstractAction::benchPlayerAttacking()
{
    return m_benchPlayerAttacking;
}

short AbstractAction::indexAttack()
{
    return m_indexAttack;
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
bool AbstractAction::checkGameManager()
{
    GameManager *manager = GameManager::instance();

    if(manager != nullptr)
    {
        m_gameManager = manager;
        return true;
    }

    qCritical() << __PRETTY_FUNCTION__ << ", manager is NULL";
    return false;
}

bool AbstractAction::checkPlayerAttacked()
{
    if(checkGameManager())
    {
        Player* playerAttacked = m_gameManager->playerAttacked();

        if(playerAttacked != nullptr)
        {
            m_playerAttacked = playerAttacked;
            return true;
        }
        else
            qCritical() << __PRETTY_FUNCTION__ << ", playerAttacked is NULL";
    }

    return false;
}

bool AbstractAction::checkPlayerAttacking()
{
    if(checkGameManager())
    {
        Player* playerAttacking = m_gameManager->currentPlayer();

        if(playerAttacking != nullptr)
        {
            m_playerAttacking = playerAttacking;
            return true;
        }
        else
            qCritical() << __PRETTY_FUNCTION__ << ", playerAttacking is NULL";
    }

    return false;
}

bool AbstractAction::checkPokemonAttached()
{
    if(m_pokemonAttached != nullptr)
        return true;

    return false;
}

bool AbstractAction::checkPokemonAttacked()
{
    if(checkPlayerAttacked())
    {
        FightArea *fightAr = m_playerAttacked->fight();

        if(fightAr != nullptr)
        {
            CardPokemon *pokemonAttacked = fightAr->pokemonFighting(0);

            if(pokemonAttacked != nullptr)
            {
                m_pokemonAttacked = pokemonAttacked;
                return true;
            }
            else
                qCritical() << __PRETTY_FUNCTION__ << ", pokemonAttacked is NULL";
        }
        else
            qCritical() << __PRETTY_FUNCTION__ << ", fightAr is NULL";
    }

    return false;
}

bool AbstractAction::checkPokemonAttacking()
{
    if(checkPlayerAttacking())
    {
        FightArea *fightAr = m_playerAttacking->fight();

        if(fightAr != nullptr)
        {
            CardPokemon *pokemonAttacking = fightAr->pokemonFighting(0);

            if(pokemonAttacking != nullptr)
            {
                m_pokemonAttacking = pokemonAttacking;
                return true;
            }
            else
                qCritical() << __PRETTY_FUNCTION__ << ", pokemonAttacking is NULL";
        }
        else
            qCritical() << __PRETTY_FUNCTION__ << ", fightAr is NULL";
    }

    return false;
}

bool AbstractAction::checkBenchPlayerAttacked()
{
    if(checkPlayerAttacked())
    {
        BenchArea* bench = m_playerAttacked->bench();

        if(bench != nullptr)
        {
            m_benchPlayerAttacked = bench;
            return true;
        }
        else
            qCritical() << __PRETTY_FUNCTION__ << ", bench is NULL";
    }

    return false;
}

bool AbstractAction::checkBenchPlayerAttacking()
{
    if(checkPlayerAttacking())
    {
        BenchArea* bench = m_playerAttacking->bench();

        if(bench != nullptr)
        {
            m_benchPlayerAttacking = bench;
            return true;
        }
        else
            qCritical() << __PRETTY_FUNCTION__ << ", bench is NULL";
    }

    return false;
}
