#include "displaydata_energiesforapokemon.h"

#include "common/database.h"
#include "src_Cards/cardpokemon.h"
#include "src_Log/log.h"
#include "src_Models/modellistenergies.h"
#include "../Share/constantesshared.h"

DisplayData_EnergiesForAPokemon::DisplayData_EnergiesForAPokemon(const QString &namePlayer, CardPokemon* pokemon, unsigned short quantity, AbstractCard::Enum_element element) :
    AbstractDisplayData(namePlayer),
    m_pokemon(pokemon),
    m_quantity(quantity),
    m_element(element)
{

}

/************************************************************
*****               FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonDocument DisplayData_EnergiesForAPokemon::messageInfoToClient()
{
    QJsonObject jsonToReturn;

    if(m_pokemon != nullptr)
    {
        jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifDisplayEnergiesForAPokemon);
        jsonToReturn["quantity"] = static_cast<short>(m_quantity);

        QJsonArray arrayPacket;
        for(int i=0;i<m_pokemon->modelListOfEnergies()->countCard();++i)
        {
            CardEnergy* energy = m_pokemon->modelListOfEnergies()->energy(i);
            if(energy != nullptr)
            {
                if((energy->element() == m_element) || (m_element == AbstractCard::Element_Whatever))
                {
                    QJsonObject objCard;
                    objCard["idEnergy"] = energy->id();
                    objCard["indexPacket"] = i;

                    arrayPacket.append(objCard);
                }
            }
        }
    }

    return QJsonDocument(jsonToReturn);
}

bool DisplayData_EnergiesForAPokemon::messageResponseFromClient(const QJsonDocument &document)
{
    if(document.isObject())
    {
        QJsonObject objJson = document.object();
        if(objJson.contains("indexPacket"))
        {
            QJsonArray arrayIndexCard = objJson["indexPacket"].toArray();
            if(arrayIndexCard.count() >= m_quantity)
            {
                QList<CardEnergy*> listEnergies;
                //Check each card
                for(int i=0;i<arrayIndexCard.count();++i)
                {
                    int indexCard = arrayIndexCard[i].toInt(-1);
                    CardEnergy* energy = m_pokemon->modelListOfEnergies()->energy(indexCard);

                    if(energy != nullptr)
                        listEnergies.append(energy);
                    else
                        m_error += "indexCard " + QString::number(indexCard) + " is not a valid index (" + QString::number(m_pokemon->modelListOfEnergies()->countCard()) + ")";
                }

                if(listEnergies.count() >= m_quantity)
                {
                    m_argument = QVariant::fromValue(listEnergies);
                    m_success = true;
                }
                else
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error " + QString::number(listEnergies.count()));
            }
            else
                m_error = "array \"indexPacket\" does not contain enough energies " + QString::number(arrayIndexCard.count()) + "/" + QString::number(m_quantity);
        }
        else
            m_error = "data \"indexPacket\" is missing";
    }
    else
        m_error = "document json is not an object";

    return m_success;
}

QJsonObject DisplayData_EnergiesForAPokemon::messageResultToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayEnergiesForAPokemonResponse);

    if(m_success)
        jsonToReturn["success"] = "ok";
    else
    {
        jsonToReturn["success"] = "ok";
        jsonToReturn["error"] = m_error;
    }

    return jsonToReturn;
}
