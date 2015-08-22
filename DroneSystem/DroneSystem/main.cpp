#include <QCoreApplication>
#include <pcsd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if(argc > 2)
    {
        int i = atoi(argv[1]);
        int p = atoi(argv[2]);
        Address adr;
        adr.ID = -1;
        adr.IP = "127.0.0.1";
        adr.PORT = 4545;
        DroneSystem DS(i, p, adr);
        DS.Work();
        std::cout << "ok\n";
    }
    else
    {
        std::cout << "else\n";
    }
    return a.exec();
}
