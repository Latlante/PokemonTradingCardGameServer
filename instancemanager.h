#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include <QObject>

class QProcess;

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

    void createNewGame(int uidPlay1, int uidPlay2, QString name = "");
    QProcess* game(int index);
    void removeGame(int index);

    QString nameOfTheGameFromUidGame(int uidGame);
    QList<int> listUidGamesFromUidPlayer(int uidPlayer);

signals:

public slots:

private:
    static InstanceManager* m_instance;

    QList<InstanceGame> m_listGame;
};

#endif // INSTANCEMANAGER_H
