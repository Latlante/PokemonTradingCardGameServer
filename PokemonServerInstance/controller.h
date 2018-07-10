#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class GameManager;
class Log;
class StdListenerWritter;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(const QString& nameGame,
                        const QString& player1,
                        const QString& player2,
                        QObject *parent = nullptr);

signals:

private:
    StdListenerWritter* m_communication;
    GameManager* m_gameManager;
    Log m_log;
};

#endif // CONTROLLER_H
