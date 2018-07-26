#include "notificationdisplaypackets.h"

NotificationDisplayPackets::NotificationDisplayPackets(const QString &namePlayer, QMap<int, int> cards) :
    AbstractNotification (ConstantesShared::PHASE_NotifDisplayPacket, namePlayer),
    m_cards(cards)
{

}

NotificationDisplayPackets::~NotificationDisplayPackets()
{

}

/************************************************************
*****				FONCTIONS PUBLIQUES					*****
************************************************************/
QJsonObject NotificationDisplayPackets::messageJsonForOwner()
{
    QJsonObject jsonResponse = initObject();

    QJsonArray arrayCards;
    for(QMap<int,int>::iterator it=m_cards.begin();it!=m_cards.end();++it)
    {
        QJsonObject objCards;
        objCards["indexPacket"] = it.key();
        objCards["idCard"] = it.value();

        arrayCards.append(objCards);
    }
    jsonResponse["packet"] = arrayCards;

    return jsonResponse;
}

QJsonObject NotificationDisplayPackets::messageJsonForOthers()
{
    return QJsonObject();
}

/************************************************************
*****				FONCTIONS PRIVEES					*****
************************************************************/
