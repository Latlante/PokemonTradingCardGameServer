#ifndef ABSTRACTNOTIFICATION_H
#define ABSTRACTNOTIFICATION_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "../Share/constantesshared.h"

class AbstractNotification
{
public:
    explicit AbstractNotification(ConstantesShared::GamePhase phase, const QString& namePlayer = "");
    virtual ~AbstractNotification();

    const QString namePlayer();
    void setNamePlayer(const QString& namePlayer);

    unsigned int indexAction();
    void setIndexAction(unsigned int indexAction);

    QByteArray messageJsonComplete();
    virtual QJsonObject messageJsonForOwner() = 0;
    virtual QJsonObject messageJsonForOthers() = 0;

protected:
    QJsonObject initObject();

private:
    ConstantesShared::GamePhase m_phase;
    QString m_namePlayer;
    unsigned int m_indexAction;


};

#endif // ABSTRACTNOTIFICATION_H
