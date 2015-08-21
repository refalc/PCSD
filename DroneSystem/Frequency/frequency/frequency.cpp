#include "frequency.h"
#include <iostream>

SysFrequency::SysFrequency(int port, QObject *parent) : m_Port(port), QObject(parent)
{
}
void SysFrequency::Start()
{
    m_Socket = new QUdpSocket(this);
    m_Socket->bind(QHostAddress::Any, m_Port);
    connect(m_Socket, SIGNAL(readyRead()), this, SLOT(Read()));
}

void SysFrequency::Close()
{
    m_Space.clear();
    m_Socket->close();
}

void SysFrequency::Read()
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
        data.IP = sender.toString().toStdString();
        data.PORT = senderPort;
        std::cout << "Data from:\n  ID = " << data.ID << "\n    IP = " << data.IP << "\n    PORT = " << data.PORT << "\n";
        m_Space[data.ID] = data;
        Send(data);
    }
}

void SysFrequency::Send(Address dst)
{
    QByteArray Data;
    Data.append(DataToStr(m_Space).c_str());
    m_Socket->writeDatagram(Data, QHostAddress(dst.IP.c_str()), dst.PORT);
}
