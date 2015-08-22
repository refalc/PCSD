#include "frequency.h"
#include <iostream>

SynFrequency::SynFrequency(int port, QObject *parent) : m_Port(port), QObject(parent)
{
}
void SynFrequency::Start()
{
    m_Socket = new QUdpSocket(this);
    m_Socket->bind(QHostAddress::Any, m_Port);
    connect(m_Socket, SIGNAL(readyRead()), this, SLOT(Read()));
    std::cout <<"Start\n";
}

void SynFrequency::Close()
{
    m_Space.clear();
    m_Socket->close();
}

void SynFrequency::Read()
{
    while(m_Socket->hasPendingDatagrams())
    {

        QByteArray buffer;
        buffer.resize(m_Socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        m_Socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);
        Address data;
        data.ID = buffer.toInt();
        data.IP = sender.toString().toStdString().substr(7);
        data.PORT = senderPort;

        if(m_Space.find(data.ID) == m_Space.end())
        {

            std::cout << "Data from:\n  ID = " << data.ID << "\n    IP = " << data.IP << "\n    PORT = " << data.PORT << "\n";
            m_Space[data.ID] = data;
        }
        Send(data);
    }
}

void SynFrequency::Send(Address dst)
{
    QByteArray Data;
    Data.append(DataToStr(m_Space).c_str());
    //std::cout << "Send to " << dst.IP.c_str()  << " PORT " << dst.PORT << " " << Data.toStdString() << std::endl;
    m_Socket->writeDatagram(Data, QHostAddress(dst.IP.c_str()), dst.PORT);
}

//tcp test

void SynFrequency::DoConnect()
{
    connect(&m_Server, SIGNAL(newConnection()),
        this, SLOT(Connected()));

    m_Server.listen(QHostAddress::Any, 8888);

}

void SynFrequency::Connected()
{
    std::cout << "Connected\n";
    m_Client = m_Server.nextPendingConnection();
    connect(m_Client, SIGNAL(readyRead()), this, SLOT(ReadTcp()));
    m_Client->write("Hello Client\n");
}

void SynFrequency::Disconnected()
{
    std::cout << "Disonnected\n";
}

void SynFrequency::ReadTcp()
{
    char buffer[1024] = {0};
    m_Client->read(buffer, m_Client->bytesAvailable());
    std::cout << buffer << endl;
    m_Client->write(buffer);
    //m_Client->close();
}

void SynFrequency::Write(qint64 bytes)
{
    std::cout << "Write " << bytes << " byte.\n";
}
