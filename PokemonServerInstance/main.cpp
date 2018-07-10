#include <QCoreApplication>
#include <QTextStream>
#include <iostream>

#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc >= 4)
    {
        Controller ctrl(argv[1], argv[2], argv[3]);
    }
    else
    {
        a.quit();
    }

    return a.exec();
}
