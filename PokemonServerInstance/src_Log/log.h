#ifndef LOG_H
#define LOG_H

#include <QObject>

class QFile;

class Log
{
public:
    explicit Log(const QString &name);
    ~Log();

    static Log* createInstance(const QString& nameFile);
    static void deleteInstance();
    static Log* instance();

    void write(QString message);

signals:

private:
    static Log* m_instance;
    QFile* m_file;
};

#endif // LOG_H
