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
        int m_uidPlayer1;
        int m_uidPlayer2;
    };

public:
    explicit InstanceManager(QObject *parent = nullptr);
    ~InstanceManager();

    static InstanceManager* instance();
    static void deleteInstance();

    unsigned int createNewGame(int uidPlay1, int uidPlay2, QString name = "");
    QProcess* game(int index);
    bool removeGame(int index);

    bool write(unsigned int uidGame, QByteArray message);

    bool checkNameOfGameIsAvailable(const QString& nameGame);
    QString nameOfTheGameFromUidGame(int uidGame);
    unsigned int uidGameFromQProcess(QProcess* process);
    QList<unsigned int> listUidGamesFromUidPlayer(int uidPlayer);

signals:
    void readyRead(unsigned int, QByteArray);

private slots:
    void onReadyRead_Process();
    void onFinished_Process(int exitCode);

private:
    static const QString m_PATH_INSTANCE;
    static InstanceManager* m_instance;
    static unsigned int m_indexGame;

    QMap<unsigned int, InstanceGame> m_listGame;
};

#endif // INSTANCEMANAGER_H
