#ifndef ABSTRACTDISPLAYDATA_H
#define ABSTRACTDISPLAYDATA_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class AbstractDisplayData : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDisplayData(QObject *parent = nullptr);

    virtual QJsonDocument messageInfoToClient() = 0;
    virtual bool messageResponseFromClient(QByteArray message) = 0;
    virtual QJsonDocument messageResultToClient() = 0;

signals:

public slots:
};

#endif // ABSTRACTDISPLAYDATA_H
