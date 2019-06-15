#include "displaydata_packet.h"

#include "src_Log/log.h"
#include "src_Packets/abstractpacket.h"
#include "../Share/constantesshared.h"

DisplayData_Packet::DisplayData_Packet(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity, AbstractCard::Enum_typeOfCard typeOfCard) :
    AbstractDisplayData (namePlayer),
    m_packet(packet),
    m_quantity(quantity),
    m_typeOfCard(typeOfCard)
{

}

/************************************************************
*****               FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonDocument DisplayData_Packet::messageInfoToClient()
{
    QJsonDocument docToReturn;

    if(m_packet != nullptr)
    {
        QJsonObject jsonDisplay;
        jsonDisplay["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifDisplayPacket);
        jsonDisplay["quantity"] = static_cast<short>(m_quantity);
        jsonDisplay["packet"] = m_packet->id();

        QJsonArray arrayCards;
        for(int i=0;i<m_packet->countCard();++i)
        {
            QJsonObject objCard;
            AbstractCard* abCard = m_packet->card(i);

            if(abCard != nullptr)
            {
                if((abCard->type() == m_typeOfCard) || (abCard->type() == AbstractCard::TypeOfCard_Whatever))
                {
                    objCard["idCard"] = abCard->id();
                    objCard["indexPacket"] = i;
                }
            }
            else
                Log::instance()->write(QString(__PRETTY_FUNCTION__) + "abCard is nullptr");
        }

        //check quantity
        if(m_quantity > arrayCards.count())
            m_quantity = arrayCards.count();
        jsonDisplay["quantity"] = m_quantity;

        docToReturn = QJsonDocument(jsonDisplay);
    }

    return docToReturn;
}

bool DisplayData_Packet::messageResponseFromClient(const QJsonDocument &document)
{
    if(document.isObject())
    {
        QJsonObject objJson = document.object();
        if(objJson.contains("indexPacket"))
        {
            QJsonArray arrayIndexPacket = objJson["indexPacket"].toArray();
            if(arrayIndexPacket.count() >= m_quantity)
            {
                QList<AbstractCard*> listCards;
                bool hasAllGoodTypeOfCard = true;
                //Check each card
                for(int i=0;i<arrayIndexPacket.count();++i)
                {
                    AbstractCard* card = m_packet->card(arrayIndexPacket[i].toInt());

                    if(card != nullptr)
                    {
                        bool hasGoodTypeOfCard = (card->type() == m_typeOfCard) || (card->type() == AbstractCard::TypeOfCard_Whatever);

                        if(hasGoodTypeOfCard == true)
                            listCards.append(card);
                        else
                            m_error += "index " + QString::number(arrayIndexPacket[i].toInt()) + " has not the good type in the packet " + m_packet->name() + ". ";

                        hasAllGoodTypeOfCard &= hasGoodTypeOfCard;
                    }
                    else
                        m_error += "index " + QString::number(arrayIndexPacket[i].toInt()) + " is not in the packet " + m_packet->name() + ". ";
                }

                if((hasAllGoodTypeOfCard == true) && (listCards.count() >= m_quantity))
                {
                    m_argument = QVariant::fromValue(listCards);
                    m_success = true;
                }
                else
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error " + QString::number(hasAllGoodTypeOfCard) + ", " + QString::number(listCards.count()));
            }
            else
                m_error = "array \"indexPacket\" does not contain enough card " + QString::number(arrayIndexPacket.count()) + "/" + QString::number(m_quantity);
        }
        else
            m_error = "data \"indexPacket\" is missing";
    }
    else
        m_error = "document json is not an object";

    return m_success;
}

QJsonObject DisplayData_Packet::messageResultToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayPacketResponse);

    if(m_success)
        jsonToReturn["success"] = "ok";
    else
    {
        jsonToReturn["success"] = "ok";
        jsonToReturn["error"] = m_error;
    }

    return jsonToReturn;
}
