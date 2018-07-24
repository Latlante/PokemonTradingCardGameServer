#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QObject>

class Authentification
{
public:
    Authentification();

    static QJsonArray listOfAllUsers();
    static QString namePlayerFromUid(unsigned int uidPlayer);

    bool checkUser(QString user, QString password);
    QString user() const;
    unsigned int uid() const;
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
    unsigned int m_uid;
    QString m_token;
};

#endif // AUTHENTIFICATION_H
