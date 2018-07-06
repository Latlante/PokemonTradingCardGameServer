#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QObject>

class Authentification
{
public:
    Authentification();

    static QList<QString> listOfAllUsers();

    bool checkUser(QString user, QString password);
    QString user() const;
    int uid() const;
    QString token() const;

private:
    static const QString m_PATH_FILE_USERS;

    enum DataPositionInFile
    {
        Position_Uid = 0,
        Position_Name,
        Position_Password
    };

    QString m_user;
    int m_uid;
    QString m_token;
};

#endif // AUTHENTIFICATION_H
