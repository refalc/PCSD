#include <QCoreApplication>
#include "frequency.h"
#include <iostream>

int main(int argc, char *argv[])
{
    int a111;
    std::cin >> a111;
    std::cout << "DDFF"<<std::endl;
    //QCoreApplication a(argc, argv);
    SysFrequency SF(4545);
    SF.Start();
    std::cout << "DDFF";
    //return a.exec();
    return 0;
}
