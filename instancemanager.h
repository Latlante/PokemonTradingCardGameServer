#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QMap>
#include <QProcess>

class InstanceManager : public QAbstractTableModel
{
    Q_OBJECT

public:
    struct InstanceGame
    {
        unsigned int uid;
        bool connected;
        QProcess* process;
        QString nameGame;
        unsigned int uidPlayer1;
        unsigned int uidPlayer2;
        QDateTime dateCreation;
    };

    enum ColumnRole
    {
        ROLE_ID = 0,
        ROLE_NAME,
        ROLE_PLAYER_CREATOR,
        ROLE_PLAYER_OPPONENT,
        ROLE_DATE_CREATION,
        ROLE_BUTTON_DELETE,
        ROLE_COUNT
    };

    explicit InstanceManager(QObject *parent = nullptr);
    virtual ~InstanceManager() override;

    static InstanceManager* instance();
    static void deleteInstance();

    //Handle game
    bool checkInstanceExeExists();
    unsigned int createNewGame(unsigned int uidPlayCreator, unsigned int uidPlayOpponent, const QString& name, QString& error);
    QProcess* game(unsigned int uidGame);
    bool removeGame(unsigned int uidGame);

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

    //Model
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual int columnCount(const QModelIndex & = QModelIndex()) const override;
    virtual int rowCount(const QModelIndex & = QModelIndex()) const override;

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

    QList<InstanceGame> m_listGames;
    //QMap<unsigned int, InstanceGame> m_listGame;

    int indexOfUidGame(unsigned int uidGame);
    void sendNotifNewGameCreated(unsigned int uidGame, unsigned int uidPlayerCreator, unsigned int uidPlayerOpponent);
};

#endif // INSTANCEMANAGER_H
