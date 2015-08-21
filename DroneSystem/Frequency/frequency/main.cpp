#include <QCoreApplication>
#include "frequency.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SysFrequency SF(4545);
    SF.Start();
    return a.exec();
}
