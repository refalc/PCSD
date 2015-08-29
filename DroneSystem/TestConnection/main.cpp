#include <QCoreApplication>
#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include "E:\soft\PCSD\DroneSystem\TestConnection\testconnection.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "startTestConnection\n";
    TestConnection s(4544);
    //s.Send("CMD_M_1:1:4|");
    s.SpamMove();
    //s.DoConnect();
   // s.SendTcp("M_0:0:10|");
    //delay(1000);
    //s.SendTcp("M_0:0:5|");

    return a.exec();
}
