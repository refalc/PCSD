#include <QCoreApplication>
#include <pcsd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if(argc > 1)
    {
        DroneSystem DS(0, 0, 0, *argv[1] - '0');
        DS.InitSys();
        std::cout << "ok\n";
    }
    else
    {
        std::cout << "else\n";
    }
    return a.exec();
}
