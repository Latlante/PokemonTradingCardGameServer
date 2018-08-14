#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include <QObject>
#include <QMap>
#include <QProcess>

class InstanceManager : public QObject
{
    Q_OBJECT
    struct InstanceGame
    {
        QProcess* process;
        QString name;
        unsigned int m_uidPlayer1;
        unsigned int m_uidPlayer2;
    };

public:
    explicit InstanceManager(QObject *parent = nullptr);
    ~InstanceManager();

    static InstanceManager* instance();
    static void deleteInstance();

    //Handle game
    bool checkInstanceExeExists();
    unsigned int createNewGame(unsigned int uidPlayCreator, unsigned int uidPlayOpponent, const QString& name, QString& error);
    QProcess* game(unsigned int index);
    bool removeGame(unsigned int index);

    //Players
    QList<unsigned int> listUidPlayersFromUidGame(unsigned int uidGame);
    bool isInTheGame(unsigned int uidGame, unsigned int uidPlayer);
    unsigned int uidOpponentOfInGame(unsigned int uidGame, unsigned int uidPlayer);
    QString nameOpponentOfInGame(unsigned int uidGame, unsigned int uidPlayer);

    //Game
    QList<unsigned int> listUidGamesFromUidPlayer(unsigned int uidPlayer);
    QString nameOfTheGameFromUidGame(unsigned int uidGame);
    bool checkNameOfGameIsAvailable(const QString& nameGame);
    unsigned int uidGameFromQProcess(QProcess* process);

signals:
    void newGameCreated(unsigned int,QString,QString,QString);
    void gameRemoved(int);
    void readyRead(unsigned int, QByteArray);

private slots:
    void onFinished_Process(int exitCode);

private:
    static const QString m_PATH_INSTANCE;
    static InstanceManager* m_instance;
    static unsigned int m_indexGame;

    QMap<unsigned int, InstanceGame> m_listGame;

    void sendNotifNewGameCreated(unsigned int uidGame, unsigned int uidPlayerCreator, unsigned int uidPlayerOpponent);
};

#endif // INSTANCEMANAGER_H
