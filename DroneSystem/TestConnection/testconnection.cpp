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
    m_Socket->writeDatagram(Data,QHostAddress("127.0.0.1"), 4545);
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

//tcp test

void TestConnection::DoConnect()
{
    m_TcpSocket = new QTcpSocket(this);

    connect(m_TcpSocket, SIGNAL(connected()),this, SLOT(Connected()));
    connect(m_TcpSocket, SIGNAL(disconnected()),this, SLOT(Disconnected()));
    connect(m_TcpSocket, SIGNAL(bytesWritten(qint64)),this, SLOT(Write(qint64)));
    connect(m_TcpSocket, SIGNAL(readyRead()),this, SLOT(ReadTcp()));

    std::cout << "connecting to TCP...\n";

    m_TcpSocket->connectToHost("127.0.0.1", 48122);

    if(!m_TcpSocket->waitForConnected(5000))
    {
        std::cout << "Error: " << m_TcpSocket->errorString().toStdString() << std::endl;
    }

}

void TestConnection::SpamMove()
{
    while(1)
    {
        delay(10000);
        Send("CMD_M_1:1:4|");
    }
}

void TestConnection::SendTcp(std::string data)
{
    if(m_Connect)
    {
        m_TcpSocket->write(data.c_str());
    }
}

void TestConnection::Connected()
{
    std::cout << "Connected\n";
    m_Connect = true;
   /* while(1)
    {
        delay(100);
        m_TcpSocket->write("HEAD / HTTP/1.0\n");
    }*/
}

void TestConnection::Disconnected()
{
    std::cout << "Disonnected\n";
}

void TestConnection::ReadTcp()
{
    std::cout << "Read: " << m_TcpSocket->readAll().toStdString() << std::endl;
}

void TestConnection::Write(qint64 bytes)
{
    std::cout << "Write " << bytes << " byte.\n";
}
