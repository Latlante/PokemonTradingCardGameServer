#include "player.h"

#include <QDebug>
#include "src_Cards/abstractcard.h"
#include "src_Cards/cardaction.h"
#include "src_Cards/cardenergy.h"
#include "src_Cards/cardpokemon.h"
#include "common/utils.h"
#include "src_Log/log.h"
#include "src_Packets/bencharea.h"
#include "src_Packets/fightarea.h"
#include "src_Packets/packetdeck.h"
#include "src_Packets/packethand.h"
#include "src_Packets/packetrewards.h"
#include "src_Packets/packettrash.h"
#include "../Share/constantesshared.h"

Player::Player(QString name, QObject *parent) :
	QObject(parent),
    m_name(name),
    m_bench(new BenchArea(NAME_BENCH)),
    m_deck(new PacketDeck(NAME_DECK)),
    m_fight(new FightArea(NAME_FIGHT)),
    m_hand(new PacketHand(NAME_HAND)),
    m_rewards(new PacketRewards(NAME_REWARDS)),
    m_trash(new PacketTrash(NAME_TRASH)),
    m_initReady(false),
    m_canPlay(true),
    m_energyPlayedForThisRound(false)
{

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

void Player::fillDeck(QList<AbstractCard*> listCards)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    foreach(AbstractCard* card, listCards)
    {
        m_deck->addNewCard(card);

        //set owner
        card->setOwner(this);

        //connection for dataChanged, energyAdded and energyRemoved
        if(card->type() == AbstractCard::TypeOfCard_Pokemon)
        {
            CardPokemon* pokemon = static_cast<CardPokemon*>(card);
            connect(pokemon, &CardPokemon::dataChanged, this, &Player::onDataChanged_CardPokemon);
            connect(pokemon, &CardPokemon::energyRemovedToTrash, this, &Player::onEnergyRemovedToTrash_CardPokemon);
        }
    }

    m_deck->mixCards();
}

void Player::emptyingDeck()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    while(m_deck->countCard() > 0)
    {
        delete m_deck->takeACard(0);
    }
}

void Player::newTurn()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    setCanPlay(true);
    m_energyPlayedForThisRound = false;
    //fight()->pokemonFighter()->setStatus(CardPokemon::Status_Normal);
}

void Player::turnFinished()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

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
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    return m_canPlay;
}

void Player::drawOneCard()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    //qDebug() << m_name << " - "<< __PRETTY_FUNCTION__;

    moveCardFromDeckToHand();
}

void Player::drawOneReward(AbstractCard* cardReward)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    moveCardFromRewardToHand(cardReward);
}

bool Player::isLoser()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

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
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    return m_initReady;
}

void Player::setInitReady(bool ready)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif
    if(m_initReady != ready)
    {
        m_initReady = ready;
        emit initReadyChanged();
    }
}

bool Player::setInitReadyIfReady()
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif
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
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

    return m_canPlay;
}

void Player::setCanPlay(bool status)
{
#ifdef TRACAGE_PRECIS
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);
#endif

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
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", indexHand:" + QString::number(indexHand) + ", indexBench:" + QString::number(indexBench));
    bool moveSuccess = false;

    AbstractCard* cardToMove = hand()->card(indexHand);

    //card cannot be moved if not pokemon in fight area
    if ((fight()->pokemonFighter() != nullptr) && (cardToMove != nullptr))
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
                                emit pokemonSwitched(name(), ConstantesShared::EnumPacketFromName(bench()->name()), indexBench, cardPok->id(), true);
                                moveSuccess = true;
                            }
                        }
                    }
                }
            }
            else
            {
                //qDebug() << __PRETTY_FUNCTION__ << ", cardPok est nullptr";
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", cardPok est nullptr, countCard hand:" + QString::number(hand()->countCard()));
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
                            if(pokemonToAssociate->addEnergy(cardEn))
                            {
                                hand()->removeFromPacketWithoutDelete(cardEn);
                                emit energyAdded(name(), ConstantesShared::PACKET_Hand, indexHand, ConstantesShared::PACKET_Bench, indexBench, cardEn->id());
                            }


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
            //qDebug() << __PRETTY_FUNCTION__ << ", cardToMove n'est pas du bon type:" << cardToMove->type();
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", cardToMove n'est pas du bon type:" + cardToMove->type());
        }
    }
    else
    {
        //qDebug() << __PRETTY_FUNCTION__ << ", cardToMove is nullptr";
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", cardToMove is nullptr");
    }

    return moveSuccess;
}

bool Player::moveCardFromHandToDeck(AbstractCard *cardHandToMove)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);

    return moveCardFromPacketToAnother(hand(), deck(), cardHandToMove);
}

bool Player::moveCardFromHandToFight(int indexHand)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", indexHand:" + QString::number(indexHand));

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
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", Evolution:" + cardPok->name());
                //On récupére la carte Pokémon a laquelle l'associer
                AbstractCard* cardToAssociate = fight()->pokemonFighter();
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", card taken");

                if (cardToAssociate != nullptr)
                {
                    if(cardToAssociate->type() == AbstractCard::TypeOfCard_Pokemon)
                    {
                        CardPokemon* pokemonToAssociate = static_cast<CardPokemon*>(cardToAssociate);

                        if (pokemonToAssociate != nullptr)
                        {
                            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", just before evolution");
                            //On l'associe au Pokémon et on peut la supprimer du paquet d'origine
                            //pour ne pas l'avoir en doublon
                            emit pokemonSwitched(name(), ConstantesShared::EnumPacketFromName(fight()->name()), 0, cardPok->id(), true);
                            if(pokemonToAssociate->evolve(cardPok))
                            {
                                Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", Evolution done");
                                hand()->removeFromPacketWithoutDelete(cardPok);
                                moveSuccess = true;
                            }
                        }
                        else
                        {
                            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + "pokemonToAssociate is nullptr");
                        }
                    }
                    else
                    {
                        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + "cardToAssociate is not a pokemon card");
                    }

                }
                else
                {
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", cardToAssociate is nullptr");
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
                            if(pokemonToAssociate->addEnergy(cardEn))
                            {
                                hand()->removeFromPacketWithoutDelete(cardEn);
                                emit energyAdded(name(), ConstantesShared::PACKET_Hand, indexHand, ConstantesShared::PACKET_Fight, 0, cardEn->id());
                            }

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
            //qDebug() << __PRETTY_FUNCTION__ << ", cardToMove n'est pas du bon type:" << cardToMove->type();
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", cardToMove n'est pas du bon type:" + cardToMove->type());

        }
    }
    else
    {
        //qDebug() << __PRETTY_FUNCTION__ << ", cardToMove is nullptr";
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", cardToMove is nullptr, countCard=" + QString::number(hand()->countCard()));
    }

    return moveSuccess;
}

bool Player::moveCardFromHandToTrash(AbstractCard* cardHandToMove)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);

    return moveCardFromPacketToAnother(hand(), trash(), cardHandToMove);
}

bool Player::moveCardFromBenchToFight(CardPokemon* pokemonToMove)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name);

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
            //qDebug() << __PRETTY_FUNCTION__ << ", cardPok n'est pas une base";
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", cardPok n'est pas une base");
        }
    }
    else
    {
        //qCritical() << __PRETTY_FUNCTION__ << ", pokemonToMove is nullptr";
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", pokemonToMove is nullptr");
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

void Player::onEnergyRemovedToTrash_CardPokemon(int indexEnergy)
{
    CardPokemon* pokemon = qobject_cast<CardPokemon*>(sender());
    int indexCard = -1;

    //check if the card come from the fight area
    indexCard = fight()->indexOf(pokemon);

    //found
    if(indexCard >= 0)
    {
        emit energyRemoved(name(), ConstantesShared::PACKET_Fight, indexCard, ConstantesShared::PACKET_Trash, 0, indexEnergy);
    }
    //try in bench area
    else
    {
        indexCard = bench()->indexOf(pokemon);

        //found
        if(indexCard >= 0)
        {
            emit energyRemoved(name(), ConstantesShared::PACKET_Bench, indexCard, ConstantesShared::PACKET_Trash, 0, indexEnergy);
        }
    }
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, int index)
{
    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", source:" + source->name() + ", destination:" + destination->name() + ", index:" + QString::number(index));

    bool moveSuccess = false;

    if (destination->isFull() == false)
    {
        //get data before action
        AbstractCard* cardToMove = source->takeACard(index);
        ConstantesShared::EnumPacket packetOrigin = ConstantesShared::EnumPacketFromName(source->name());
        ConstantesShared::EnumPacket packetDestination = ConstantesShared::EnumPacketFromName(destination->name());
        int idCard = cardToMove->id();

        if (cardToMove != nullptr)
        {
            destination->addNewCard(cardToMove);
            moveSuccess = true;

            //Send notification
            if((destination->name() == NAME_BENCH) || (destination->name() == NAME_FIGHT))
                emit cardMoved(name(), packetOrigin, index, packetDestination, idCard, true);
            else
                emit cardMoved(name(), packetOrigin, index, packetDestination, idCard, false);
        }
        else
        {
            //qCritical() << __PRETTY_FUNCTION__ << "Card is nullptr";
            Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + "Card is nullptr");

        }
    }

    return moveSuccess;
}

bool Player::moveCardFromPacketToAnother(AbstractPacket *source, AbstractPacket *destination, AbstractCard *cardToMove)
{
    bool moveSuccess = false;

    if((source != nullptr) && (destination != nullptr) && (cardToMove != nullptr))
    {
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", source:" + source->name() + ", destination:" + destination->name() + ", cardToMove:" + cardToMove->name());

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
                if((destination->name() == NAME_BENCH) || (destination->name() == NAME_FIGHT))
                    emit cardMoved(name(), packetOrigin, indexCardOrigin, packetDestination, idCard, true);
                else
                    emit cardMoved(name(), packetOrigin, indexCardOrigin, packetDestination, idCard, false);
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

        //qCritical() << __PRETTY_FUNCTION__ << messageError;
        Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", name:" + m_name + ", error:" + messageError);
    }

    return moveSuccess;
}
