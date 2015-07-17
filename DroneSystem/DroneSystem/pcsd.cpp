#include <pcsd.h>
#include <iostream>

DroneSystem::DroneSystem(address init_space, address syn_space, address coord_space, int ID)
{
    m_ID = ID;
    m_InitSpace = init_space;
    udpSocket = new QUdpSocket();
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
}

int DroneSystem::InitSys()
{
    char buff[6];
    QByteArray datagram = itoa(m_ID, buff, 10);
    QHostAddress Address = QHostAddress("127.0.0.1");
    udpSocket->writeDatagram(datagram, Address, 45454);
    datagram.clear();
    while (udpSocket->hasPendingDatagrams()) {
            datagram.resize(udpSocket->pendingDatagramSize());
            udpSocket->readDatagram(datagram.data(), datagram.size());
            std::cout << datagram.data() << std::endl;
        }
}
