#include "frequency.h"
#include <iostream>
#include <algorithm>

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

        //Decode input data
        if(buffer.toStdString().find("CMD_") == std::string::npos)
        {
            //std::cout << "Recive DATA from Drone\n";
            QString strID, strPORT;
            bool state = false;

            for(auto it = buffer.begin(); it != buffer.end(); it++)
            {
                if(!state)
                {
                    if(*it == ' ')
                        state = true;
                    else
                        strID.append(*it);
                }
                else
                {
                    strPORT.append(*it);
                }
            }
            //

            data.m_iID = strID.toInt();
            data.m_IP = sender.toString().toStdString().substr(7);
            data.m_iPort = strPORT.toInt();

            if(m_Space.find(data.m_iID) == m_Space.end())
            {
                m_Space[data.m_iID] = data;
                data.m_iPort = senderPort;
                std::cout << "Data from:\n  ID = " << data.m_iID << "\n    IP = " << data.m_IP << "\n    PORT = " << data.m_iPort << "\n";
            }
            data.m_iPort = senderPort;
            Send(data);
        }
        else
        {
            //Recive COMMAND
            std::cout << "Recive COMMAND\n";
            Send(buffer.toStdString().substr(4));
        }
    }
}

void SynFrequency::Send(const Address &dst)
{
    QByteArray Data;
    Data.append(DataToStr(m_Space).c_str());
    //std::cout << "Send to " << dst.IP.c_str()  << " PORT " << dst.PORT << " " << Data.toStdString() << std::endl;
    m_Socket->writeDatagram(Data, QHostAddress(dst.m_IP.c_str()), dst.m_iPort);
}

void SynFrequency::Send(const std::string &data)
{
    QByteArray Data(data.c_str());
    for(auto it = m_Space.begin(), it_end = m_Space.end(); it != it_end; it++)
    {
        m_Socket->writeDatagram(Data, QHostAddress(it->second.m_IP.c_str()), it->second.m_iPort-1);
    }
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
    int size = m_Clients.size();
    m_Clients.push_back(m_Server.nextPendingConnection());
    connect(m_Clients[size], SIGNAL(readyRead()), this, SLOT(ReadTcp(int)));
    m_Clients[size]->write("Hello Client\n");
}

void SynFrequency::Disconnected()
{
    std::cout << "Disonnected\n";
}

void SynFrequency::ReadTcp(int pos)
{
    char buffer[1024] = {0};
    m_Clients[pos]->read(buffer, m_Client->bytesAvailable());
    std::cout << buffer << endl;
    m_Clients[pos]->write(buffer);
    //m_Client->close();
}

void SynFrequency::Write(qint64 bytes)
{
    std::cout << "Write " << bytes << " byte.\n";
}
