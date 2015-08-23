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

            data.ID = strID.toInt();
            data.IP = sender.toString().toStdString().substr(7);
            data.PORT = strPORT.toInt();

            if(m_Space.find(data.ID) == m_Space.end())
            {
                m_Space[data.ID] = data;
                data.PORT = senderPort;
                std::cout << "Data from:\n  ID = " << data.ID << "\n    IP = " << data.IP << "\n    PORT = " << data.PORT << "\n";
            }
            data.PORT = senderPort;
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

void SynFrequency::Send(Address dst)
{
    QByteArray Data;
    Data.append(DataToStr(m_Space).c_str());
    //std::cout << "Send to " << dst.IP.c_str()  << " PORT " << dst.PORT << " " << Data.toStdString() << std::endl;
    m_Socket->writeDatagram(Data, QHostAddress(dst.IP.c_str()), dst.PORT);
}

void SynFrequency::Send(std::string data)
{
    QByteArray Data(data.c_str());
    for(auto it = m_Space.begin(), it_end = m_Space.end(); it != it_end; it++)
    {
        m_Socket->writeDatagram(Data, QHostAddress(it->second.IP.c_str()), it->second.PORT-1);
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
