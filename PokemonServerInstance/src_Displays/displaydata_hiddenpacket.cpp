#include "displaydata_hiddenpacket.h"

#include "src_Cards/abstractcard.h"
#include "src_Log/log.h"
#include "src_Packets/abstractpacket.h"
#include "../Share/constantesshared.h"

DisplayData_HiddenPacket::DisplayData_HiddenPacket(const QString &namePlayer, AbstractPacket *packet, unsigned short quantity) :
    AbstractDisplayData(namePlayer),
    m_packet(packet),
    m_quantity(quantity)
{

}

/************************************************************
*****               FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonDocument DisplayData_HiddenPacket::messageInfoToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_NotifDisplayHiddenPacket);
    jsonToReturn["numberOfCards"] = m_packet->countCard();
    jsonToReturn["quantity"] = static_cast<short>(m_quantity);

    return QJsonDocument(jsonToReturn);
}

bool DisplayData_HiddenPacket::messageResponseFromClient(const QJsonDocument &document)
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
                //Check each card
                for(int i=0;i<arrayIndexPacket.count();++i)
                {
                    AbstractCard* card = m_packet->card(arrayIndexPacket[i].toInt());

                    if(card != nullptr)
                        listCards.append(card);
                    else
                        m_error += "index " + QString::number(arrayIndexPacket[i].toInt()) + " is not in the packet " + m_packet->name() + ". ";
                }

                if(listCards.count() >= m_quantity)
                {
                    m_argument = QVariant::fromValue(listCards);
                    m_success = true;
                }
                else
                    Log::instance()->write(QString(__PRETTY_FUNCTION__) + ", error, no enough card in packet checked:" + QString::number(listCards.count()) + "/" + QString::number(m_quantity));
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

QJsonObject DisplayData_HiddenPacket::messageResultToClient()
{
    QJsonObject jsonToReturn;

    jsonToReturn["phase"] = static_cast<int>(ConstantesShared::PHASE_DisplayHiddenPacketResponse);

    if(m_success)
    {
        QJsonArray array;
        QList<AbstractCard*> listCards = m_argument.value<QList<AbstractCard*> >();
        foreach(AbstractCard* abCard, listCards)
            array.push_back(abCard->id());
        jsonToReturn["idCards"] = array;
        jsonToReturn["success"] = "ok";
    }
    else
    {
        jsonToReturn["success"] = "ok";
        jsonToReturn["error"] = m_error;
    }

    return jsonToReturn;
}
