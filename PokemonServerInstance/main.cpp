#include <QCoreApplication>
#include <QTextStream>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc >= 4)
    {

    }

    QTextStream sin(stdin);
    QTextStream sout(stdout);

    sout << "argc: " << argc << endl;
    for(int i=0;i<argc;++i)
        sout << "argv: " <<  argv[i] << endl;

    return a.exec();
}
