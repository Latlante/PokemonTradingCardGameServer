#ifndef ABSTRACTNOTIFICATION_H
#define ABSTRACTNOTIFICATION_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "../Share/constantesshared.h"

class AbstractNotification
{
    Q_OBJECT
public:
    explicit AbstractNotification(const QString& namePlayer = "");

    const QString namePlayer();
    void setNamePlayer(const QString& namePlayer);

    QByteArray messageJsonComplete();

signals:

private:
    QString m_namePlayer;

    virtual QJsonObject messageJsonForOwner() = 0;
    virtual QJsonObject messageJsonForOthers() = 0;

};

#endif // ABSTRACTNOTIFICATION_H
