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
    //s.Send("7");
    s.DoConnect();

    return a.exec();
}
