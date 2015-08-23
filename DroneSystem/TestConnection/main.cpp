#include <QCoreApplication>
#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include "E:\soft\PCSD\DroneSystem\TestConnection\testconnection.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "start\n";
    TestConnection s(4544);
    s.Send("CMD_M_1:1:4|");
    //s.DoConnect();

    return a.exec();
}
