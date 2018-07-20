#include "player.h"

#include <QDebug>
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "common/utils.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packethand.h"
#include "src_Packets/packetrewards.h"
#include "src_Packets/packettrash.h"
#include "../Share/constantesshared.h"

Player::Player(QString name, QList<AbstractCard*> listCards, QObject *parent) :
	QObject(parent),
    m_name(name),
    m_bench(new BenchArea(NAME_BENCH)),
    m_deck(new PacketDeck(NAME_DECK, listCards)),
    m_fight(new FightArea(NAME_FIGHT)),
    m_hand(new PacketHand(NAME_HAND)),
    m_rewards(new PacketRewards(NAME_REWARDS)),
    m_trash(new PacketTrash(NAME_TRASH)),
    m_initReady(false),
    m_canPlay(true),
    m_energyPlayedForThisRound(false)
{
    foreach(AbstractCard* card, listCards)
    {
        //set owner
        card->setOwner(this);

        //connection for dataChanged, energyAdded and energyRemoved
        if(card->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(card);
            connect(pokemon, &CardPokemon::dataChanged, this, &Player::onDataChanged_CardPokemon);
            connect(pokemon, &CardPokemon::energyAdded, this, &Player::onEnergyAdded_CardPokemon);
            connect(pokemon, &CardPokemon::energyRemoved, this, &Player::onEnergyRemoved_CardPokemon);
        }
    }
}

Player::~Player()
{
    delete m_bench;
    delete m_deck;
    delete m_fight;
    delete m_hand;
    delete m_rewards;
    delete m_trash;
}

/************************************************************
*****				FONCTIONS STATIQUES					*****
************************************************************/

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
const QString Player::name()
{
    return m_name;
}

BenchArea* Player::bench()
{
    return m_bench;
}

PacketDeck* Player::deck()
{
    return m_deck;
}

FightArea* Player::fight()
{
    return m_fight;
}

PacketHand* Player::hand()
{
    return m_hand;
}

PacketRewards* Player::rewards()
{
    return m_rewards;
}

PacketTrash* Player::trash()
{
    return m_trash;
}

void Player::newTurn()
{
    setCanPlay(true);
    m_energyPlayedForThisRound = false;
    //fight()->pokemonFighter()->setStatus(CardPokemon::Status_Normal);
}

void Player::turnFinished()
{
    //On remet à 0 les historiques de dégats pour chaque pokémon
    for(int i=0;i<fight()->countCard();++i)
    {
        fight()->pokemonFighting(i)->resetLastDamageReceived();
    }

    for(int i=0;i<bench()->countCard();++i)
    {
        bench()->cardPok(i)->resetLastDamageReceived();
    }

    //On bloque l'interface
    setCanPlay(false);
}

bool Player::isPlaying()
{
    return m_canPlay;
}

void Player::drawOneCard()
{
    //qDebug() << m_name << " - "<< __PRETTY_FUNCTION__;

    moveCardFromDeckToHand();
}

void Player::drawOneReward(AbstractCard* cardReward)
{
    moveCardFromRewardToHand(cardReward);
}

bool Player::isLoser()
{
    //Conditions de victoire:
    //  -> Plus de récompense à piocher
    //  -> Plus de carte dans le deck
    //  -> Plus de pokémon sur la banc
    bool hasAWinner = false;

    hasAWinner |= rewards()->isEmpty();
    hasAWinner |= deck()->isEmpty();
    hasAWinner |= bench()->isEmpty() && fight()->isEmpty();

    return hasAWinner;
}

bool Player::initReady()
{
    return m_initReady;
}

void Player::setInitReady(bool ready)
{
    if(m_initReady != ready)
    {
        m_initReady = ready;
        emit initReadyChanged();
    }
}

bool Player::setInitReadyIfReady()
{
    bool success = false;

    if(fight()->countCard() > 0)
    {
        setInitReady(true);
        setCanPlay(false);
        success = true;
    }

    return success;
}

bool Player::canPlay()
{
    return m_canPlay;
}

void Player::setCanPlay(bool status)
{
    if(status != m_canPlay)
    {
        m_canPlay = status;
        emit canPlayChanged();
    }
}

bool Player::moveCardFromDeckToHand(AbstractCard *cardDeckToMove)
{
    if(cardDeckToMove == nullptr)
        return moveCardFromPacketToAnother(deck(), hand(), deck()->card(0));

    return moveCardFromPacketToAnother(deck(), hand(), cardDeckToMove);
}

bool Player::moveCardFromDeckToReward(AbstractCard* cardDeckToMove)
{
    if(cardDeckToMove == nullptr)
        return moveCardFromPacketToAnother(deck(), rewards(), deck()->card(0));

    return moveCardFromPacketToAnother(deck(), rewards(), cardDeckToMove);
}

bool Player::moveCardFromHandToBench(int indexHand, int indexBench)
{
    bool moveSuccess = false;

    AbstractCard* cardToMove = hand()->card(indexHand);

    if (cardToMove != nullptr)
    {
        //On autorise uniquement les cartes de type Pokemon a être posé sur le banc
        if (cardToMove->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* cardPok = static_cast<CardPokemon*>(cardToMove);

            if(cardPok != nullptr)
            {
                //Pokémon de base
                if(cardPok->isBase() == true)
                {
                    moveSuccess = moveCardFromPacketToAnother(hand(), bench(), indexHand);
                }
                //Evolution
                else
                {
                    //On récupére la carte Pokémon a laquelle l'associer
                    AbstractCard* cardToAssociate = bench()->card(indexBench);

                    if ((cardToAssociate != nullptr) && (cardToAssociate->type() == AbstractCard::TypeOfCard_Pokemon))
                    {
                        CardPokemon* pokemonToAssociate = static_cast<CardPokemon*>(cardToAssociate);

                        if (pokemonToAssociate != nullptr)
                        {
                            //On l'associe au Pokémon et on peut la supprimer du paquet d'origine
                            //pour ne pas l'avoir en doublon
                            if(pokemonToAssociate->evolve(cardPok))
                            {
                                hand()->removeFromPacketWithoutDelete(cardPok);
                                moveSuccess = true;
                            }
                        }
                    }
                }
            }
            else
            {
                qDebug() << __PRETTY_FUNCTION__ << ", cardPok est nullptr";
            }
        }
        else if (cardToMove->type() == AbstractCard::TypeOfCard_Energy)
        {
            if((m_energyPlayedForThisRound == false) && (m_initReady == true))
            {
                CardEnergy* cardEn = static_cast<CardEnergy*>(cardToMove);

                if (cardEn != nullptr)
                {
                    //On récupére la carte Pokémon a laquelle l'associer
                    AbstractCard* cardToAssociate = bench()->card(indexBench);

                    if ((cardToAssociate != nullptr) && (cardToAssociate->type() == AbstractCard::TypeOfCard_Pokemon))
                    {
                        CardPokemon* pokemonToAssociate = static_cast<CardPokemon*>(cardToAssociate);

                        if (pokemonToAssociate != nullptr)
                        {
                            //On l'associe au Pokémon et on peut la supprimer du paquet d'origine
                            //pour ne pas l'avoir en doublon
                            pokemonToAssociate->addEnergy(cardEn);
                            hand()->removeFromPacketWithoutDelete(cardEn);

                            m_energyPlayedForThisRound = true;
                            moveSuccess = true;
                        }
                    }
                }
            }
        }
        else if (cardToMove->type() == AbstractCard::TypeOfCard_Action)
        {
            CardAction* cardTrainer = static_cast<CardAction*>(cardToMove);

            if(cardTrainer != nullptr)
            {
                moveSuccess = moveCardFromHandToTrash(cardTrainer);

                if(moveSuccess == true)
                    cardTrainer->executeAction(bench()->cardPok(indexBench));
            }
        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << ", cardToMove n'est pas du bon type:" << cardToMove->type();
        }
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ", cardToMove is nullptr";
    }

    return moveSuccess;
}

bool Player::moveCardFromHandToDeck(AbstractCard *cardHandToMove)
{
    return moveCardFromPacketToAnother(hand(), deck(), cardHandToMove);
}

bool Player::moveCardFromHandToFight(int indexHand)
{
    bool moveSuccess = false;

    AbstractCard* cardToMove = hand()->card(indexHand);

    if (cardToMove != nullptr)
    {
        //On autorise uniquement les cartes de type Pokemon a être posé sur le banc
        if (cardToMove->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* cardPok = static_cast<CardPokemon*>(cardToMove);

            //Pokémon de base
            if(cardPok->isBase() == true)
            {
                moveSuccess = moveCardFromPacketToAnother(hand(), fight(), indexHand);
            }
            //Evolution
            else
            {
                //On récupére la carte Pokémon a laquelle l'associer
                AbstractCard* cardToAssociate = fight()->pokemonFighter();

                if ((cardToAssociate != nullptr) && (cardToAssociate->type() == AbstractCard::TypeOfCard_Pokemon))
                {
                    CardPokemon* pokemonToAssociate = static_cast<CardPokemon*>(cardToAssociate);

                    if (pokemonToAssociate != nullptr)
                    {
                        //On l'associe au Pokémon et on peut la supprimer du paquet d'origine
                        //pour ne pas l'avoir en doublon
                        if(pokemonToAssociate->evolve(cardPok))
                        {
                            hand()->removeFromPacketWithoutDelete(cardPok);
                            moveSuccess = true;
                        }
                    }
                }
            }
        }
        else if (cardToMove->type() == AbstractCard::TypeOfCard_Energy)
        {
            if((m_energyPlayedForThisRound == false) && (m_initReady == true))
            {
                CardEnergy* cardEn = static_cast<CardEnergy*>(cardToMove);

                if (cardEn != nullptr)
                {
                    //On récupére la carte Pokémon a laquelle l'associer
                    AbstractCard* cardToAssociate = fight()->pokemonFighter();

                    if ((cardToAssociate != nullptr) && (cardToAssociate->type() == AbstractCard::TypeOfCard_Pokemon))
                    {
                        CardPokemon* pokemonToAssociate = static_cast<CardPokemon*>(cardToAssociate);

                        if (pokemonToAssociate != nullptr)
                        {
                            //On l'associe au Pokémon et on peut la supprimer du paquet d'origine
                            //pour ne pas l'avoir en doublon
                            pokemonToAssociate->addEnergy(cardEn);
                            hand()->removeFromPacketWithoutDelete(cardEn);

                            m_energyPlayedForThisRound = true;
                            moveSuccess = true;
                        }
                    }
                }
            }

        }
        else if (cardToMove->type() == AbstractCard::TypeOfCard_Action)
        {
            CardAction* cardTrainer = static_cast<CardAction*>(cardToMove);

            if(cardTrainer != nullptr)
            {
                moveSuccess = moveCardFromHandToTrash(cardTrainer);

                if(moveSuccess == true)
                    cardTrainer->executeAction(fight()->pokemonFighting(0));
            }
        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << ", cardToMove n'est pas du bon type:" << cardToMove->type();
        }
    }
    else
    {
        qDebug() << __PRETTY_FUNCTION__ << ", cardToMove is nullptr";
    }

    return moveSuccess;
}

bool Player::moveCardFromHandToTrash(AbstractCard* cardHandToMove)
{
    return moveCardFromPacketToAnother(hand(), trash(), cardHandToMove);
}

bool Player::moveCardFromBenchToFight(CardPokemon* pokemonToMove)
{
    bool moveSuccess = false;

    if (pokemonToMove != nullptr)
    {
        //On refuse les évolutions
        if (pokemonToMove->isBase() == true)
        {
            moveSuccess = moveCardFromPacketToAnother(bench(), fight(), pokemonToMove);
        }
        else
        {
            qDebug() << __PRETTY_FUNCTION__ << ", cardPok n'est pas une base";
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << ", pokemonToMove is nullptr";
    }

    return moveSuccess;
}

bool Player::moveCardFromBenchToTrash(int index)
{
    CardPokemon* pokemonBench = bench()->cardPok(index);
    QList<AbstractCard*> listCardAttached = pokemonBench->purge();

    foreach(AbstractCard* card, listCardAttached)
        trash()->addNewCard(card);

    return moveCardFromPacketToAnother(bench(), trash(), index);
}

bool Player::moveCardFromFightToTrash(int index)
{
    CardPokemon* pokemonFighter = fight()->pokemonFighting(index);
    QList<AbstractCard*> listCardAttached = pokemonFighter->purge();

    foreach(AbstractCard* card, listCardAttached)
        trash()->addNewCard(card);

    return moveCardFromPacketToAnother(fight(), trash(), index);
}

bool Player::moveCardFromRewardToHand(AbstractCard* cardReward)
{
    return moveCardFromPacketToAnother(rewards(), hand(), cardReward);
}

bool Player::moveCardFromTrashToHand(AbstractCard *cardTrash)
{
    return moveCardFromPacketToAnother(trash(), hand(), cardTrash);
}

bool Player::moveAllCardFromHandToDeck()
{
    bool status = true;

    while((hand()->countCard() > 0) && (status == true))
    {
        status &= moveCardFromHandToDeck(hand()->card(0));
    }

    return status;
}

bool Player::swapCardsBetweenBenchAndFight(CardPokemon* pokemonBenchToSwap)
{
    bool status = true;

    if((fight() != nullptr) && (bench() != nullptr))
    {
        if((fight()->pokemonFighting(0) != nullptr) && (pokemonBenchToSwap != nullptr))
        {
            //A ce moment là, on sait que tout est bon
            //On commence donc par récupérer le pokémon qui se bat pour le remplace
            AbstractCard* pokFigther = fight()->takeACard(0);
            status &= moveCardFromBenchToFight(pokemonBenchToSwap);
            status &= bench()->addNewCard(pokFigther);
        }
        else
            status = false;
    }
    else
        status = false;

    return status;
}

/************************************************************
*****			 FONCTIONS SLOTS PRIVEES				*****
************************************************************/
void Player::onDataChanged_CardPokemon()
{
    CardPokemon* pokemon = qobject_cast<CardPokemon*>(sender());
    int indexCard = -1;

    //check if the card come from the fight area
    indexCard = fight()->indexOf(pokemon);

    //found
    if(indexCard >= 0)
    {
        emit dataPokemonChanged(name(), ConstantesShared::PACKET_Fight, indexCard, pokemon);
    }
    //try in bench area
    else
    {
        indexCard = bench()->indexOf(pokemon);

        //found
        if(indexCard >= 0)
        {
            emit dataPokemonChanged(name(), ConstantesShared::PACKET_Bench, indexCard, pokemon);
        }
    }
}

void Player::onEnergyAdded_CardPokemon(int idEnergy)
{
    CardPokemon* pokemon = qobject_cast<CardPokemon*>(sender());
    int indexCard = -1;

    //check if the card come from the fight area
    indexCard = fight()->indexOf(pokemon);

    //found
    if(indexCard >= 0)
    {
        emit energyAdded(name(), ConstantesShared::PACKET_Fight, indexCard, idEnergy);
    }
    //try in bench area
    else
    {
        indexCard = bench()->indexOf(pokemon);

        //found
        if(indexCard >= 0)
        {
            emit energyAdded(name(), ConstantesShared::PACKET_Bench, indexCard, idEnergy);
        }
    }
}

void Player::onEnergyRemoved_CardPokemon(int indexEnergy)
{
    CardPokemon* pokemon = qobject_cast<CardPokemon*>(sender());
    int indexCard = -1;

    //check if the card come from the fight area
    indexCard = fight()->indexOf(pokemon);

    //found
    if(indexCard >= 0)
    {
        emit energyRemoved(name(), ConstantesShared::PACKET_Fight, indexCard, indexEnergy);
    }
    //try in bench area
    else
    {
        indexCard = bench()->indexOf(pokemon);

        //found
        if(indexCard >= 0)
        {
            emit energyRemoved(name(), ConstantesShared::PACKET_Bench, indexCard, indexEnergy);
        }
    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, int index)
{
    bool moveSuccess = false;

    if (destination->isFull() == false)
    {
        AbstractCard* cardToMove = source->takeACard(index);

        if (cardToMove != nullptr)
        {
            destination->addNewCard(cardToMove);
            moveSuccess = true;
        }
        else
        {
            qCritical() << __PRETTY_FUNCTION__ << "Card is nullptr";

        }
    }

    return moveSuccess;
}

bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, AbstractCard *cardToMove)
{
    bool moveSuccess = false;

    if((source != nullptr) && (destination != nullptr) && (cardToMove != nullptr))
    {
        if (destination->isFull() == false)
        {
            //get data before action
            ConstantesShared::EnumPacket packetOrigin = ConstantesShared::EnumPacketFromName(source->name());
            int indexCardOrigin = source->indexOf(cardToMove);
            ConstantesShared::EnumPacket packetDestination = ConstantesShared::EnumPacketFromName(destination->name());
            int idCard = cardToMove->id();

            //action
            moveSuccess = source->removeFromPacketWithoutDelete(cardToMove);

            if(moveSuccess == true)
            {
                moveSuccess = destination->addNewCard(cardToMove);

                //Send notification
                emit cardMoved(name(), packetOrigin, indexCardOrigin, packetDestination, idCard);
            }
        }
    }
    else
    {
        QString messageError = "Element(s) is/are nullptr:\n";
        if(source == nullptr)
            messageError += "  - err: source is nullptr";
        else
            messageError += "  - source is " + source->name();

        if(destination == nullptr)
            messageError += "  - err: destination is nullptr";
        else
            messageError += "  - destination is " + destination->name();

        if(cardToMove == nullptr)
            messageError += "  - err: cardToMove is nullptr";
        else
            messageError += "  - source is " + cardToMove->name();

        qCritical() << __PRETTY_FUNCTION__ << messageError;

    }

    return moveSuccess;
}
