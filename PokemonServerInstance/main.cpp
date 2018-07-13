#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QResource>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /*QFile fichier("Test.txt");
    fichier.open(QIODevice::WriteOnly | QIODevice::Text);
    fichier.write("Hello");
    fichier.close();

    qDebug() << "argc=" << argc;
    for(int i=0;i<argc;++i)
        qDebug() << i << ": " << argv[i];*/

    if((argc >= 4) && (QFile::exists("Resources/cards.rcc")))
    {
        qDebug() << "OK";
        QResource::registerResource("Resources/cards.rcc");

        Controller ctrl(argv[1], argv[2], argv[3]);
        return a.exec();
    }
    else
    {
        qDebug() << "KO";
        a.quit();
    }

    return 0;
}
