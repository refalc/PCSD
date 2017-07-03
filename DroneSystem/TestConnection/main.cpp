#include <QCoreApplication>
#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include "testconnection.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "startTestConnection\n";
    if( argc == 5 ) {
        std::string type = argv[1];
        if( !type.compare("sender") ) {
            int cur_port = atoi(argv[2]);
            Address adr;
            adr.m_iID = -1;
            adr.m_IP = "127.0.0.1";
            adr.m_iPort = atoi(argv[3]);
            std::string socket_type = argv[4];
            if( !socket_type.compare("TCP") ) {
                CTestConnectionSender sender(SocketType::TCP_Client, cur_port, adr);
                sender.Work();
            } else {
                std::cout << "TODO\n";
            }
        } else if( !type.compare("receiver") ) {
            int port_1 = atoi(argv[2]);
            int port_2 = atoi(argv[3]);
            int port_tcp = atoi(argv[4]);
            CTestConnectionReciever receiver(port_1, port_2, port_tcp);
            receiver.Work();
        } else {
            std::cout << "Invalid args\n";
        }
    } else
        std::cout << "Invalid numbers of args\n";


    return a.exec();
}
