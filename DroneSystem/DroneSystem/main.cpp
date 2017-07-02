#include <QCoreApplication>
#include <pcsd.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    if(argc > 3)
    {
        int i = atoi(argv[1]);
        int p = atoi(argv[2]);
        int tcp_p = atoi(argv[3]);
        Address adr;
        adr.m_iID = -1;
        adr.m_IP = "127.0.0.1";
        adr.m_iPort = 4545;
        DroneSystem DS(i, p, tcp_p, adr);
        DS.Work();
        std::cout << "ok\n";
    }
    else
    {
        std::cout << "else\n";
    }
    return a.exec();
}
