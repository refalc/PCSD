#include <QCoreApplication>
#include "frequency.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SynFrequency SF(4545);
    SF.Start();
    //SF.DoConnect();

    return a.exec();
}
