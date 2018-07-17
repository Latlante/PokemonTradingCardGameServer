#ifndef HISTORICALNOTIFICATIONS_H
#define HISTORICALNOTIFICATIONS_H

#include <QObject>

class AbstractNotification;

class HistoricalNotifications : public QObject
{
    Q_OBJECT
public:
    explicit HistoricalNotifications(QObject *parent = nullptr);

    int addNewNotification(AbstractNotification* notif);

signals:

private:
    QList<AbstractNotification*> m_listNotifications;
};

#endif // HISTORICALNOTIFICATIONS_H
