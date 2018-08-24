#include "displaydata_allelements.h"

#include "common/database.h"
#include "src_Log/log.h"
#include "../Share/constantesshared.h"

DisplayData_AllElements::DisplayData_AllElements(const QString &namePlayer, unsigned short quantity) :
    AbstractDisplayData(namePlayer),
    m_quantity(quantity)
{

}

/************************************************************
*****               FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonDocument DisplayData_AllElements::messageInfoToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifDisplayAllElements);
    jsonToReturn["quantity"] = static_cast<short>(m_quantity);

    Database db;
    QList<int> listAllIdEnergies = db.listIdAllCardsEnergies();
    QJsonArray arrayIdEnergies;

    foreach(int idEnergy, listAllIdEnergies)
        arrayIdEnergies.append(idEnergy);
    jsonToReturn["elements"] = arrayIdEnergies;

    return QJsonDocument(jsonToReturn);
}

bool DisplayData_AllElements::messageResponseFromClient(const QJsonDocument &document)
{
    if(document.isObject())
    {
        QJsonObject objJson = document.object();
        if(objJson.contains("elements"))
        {
            QJsonArray arrayIdsElements = objJson["elements"].toArray();
            if(arrayIdsElements.count() >= m_quantity)
            {
                Database db;
                QList<AbstractCard::Enum_element> listElements;
                //Check each card
                for(int i=0;i<arrayIdsElements.count();++i)
                {
                    int idElement = arrayIdsElements[i].toInt(0);
                    AbstractCard* card = db.cardById(idElement);
                    if(card != nullptr)
                    {
                        if(card->type() == AbstractCard::TypeOfCard_Energy)
                        {
                            CardEnergy *energy = static_cast<CardEnergy*>(card);
                            listElements.append(energy->element());
                        }
                        else
                            m_error += "idElement " + QString::number(idElement) + " is not a energy card";

                        delete card;
                    }
                    else
                        m_error += "idElement " + QString::number(idElement) + " is not a valid id";
                }

                if(listElements.count() >= m_quantity)
                {
                    m_argument = QVariant::fromValue(listElements);
                    m_success = true;
                }
                else
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error " + QString::number(listElements.count()));
            }
            else
                m_error = "array \"elements\" does not contain enough energies " + QString::number(arrayIdsElements.count()) + "/" + QString::number(m_quantity);
        }
        else
            m_error = "data \"elements\" is missing";
    }
    else
        m_error = "document json is not an object";

    return m_success;
}

QJsonObject DisplayData_AllElements::messageResultToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayAllElementsResponse);

    if(m_success)
        jsonToReturn["success"] = "ok";
    else
    {
        jsonToReturn["success"] = "ok";
        jsonToReturn["error"] = m_error;
    }

    return jsonToReturn;
}
