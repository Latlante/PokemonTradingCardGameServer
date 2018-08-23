#ifndef ABSTRACTDISPLAYDATA_H
#define ABSTRACTDISPLAYDATA_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

class AbstractDisplayData : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDisplayData(const QString &namePlayer);
    virtual ~AbstractDisplayData();

    bool success();
    QString error() const;
    QVariant argument() const;

    virtual QJsonDocument messageInfoToClient() = 0;
    virtual bool messageResponseFromClient(const QJsonDocument &document) = 0;
    virtual QJsonObject messageResultToClient() = 0;

protected:
    QString m_namePlayer;
    bool m_success;
    QString m_error;
    QVariant m_argument;

public slots:
};

#endif // ABSTRACTDISPLAYDATA_H
