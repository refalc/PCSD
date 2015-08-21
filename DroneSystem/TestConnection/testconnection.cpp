#include "E:\soft\PCSD\DroneSystem\TestConnection\testconnection.h"

TestConnection::TestConnection(int port, QObject *parent) : m_Port(port), QObject(parent)
{
    m_Socket = new QUdpSocket(this);
    m_Socket->bind(QHostAddress::LocalHost, m_Port);
    connect(m_Socket, SIGNAL(readyRead()), this, SLOT(Read()));
}
void TestConnection::Send(std::string ID)
{
    QByteArray Data;
    Data.append(ID.c_str());
    m_Socket->writeDatagram(Data, QHostAddress::LocalHost, 4545);
    std::cout << "Send to " << 4545 << std::endl;
}

void TestConnection::Read()
{
    while(m_Socket->hasPendingDatagrams())
    {

        QByteArray buffer;
        buffer.resize(m_Socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        m_Socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
        std::cout << buffer.toStdString() << std::endl;
        std::string tempStr = buffer.toStdString();
        m_Space = StrToData(tempStr);
    }
}
TestConnection::~TestConnection()
{
    m_Socket->close();
}
