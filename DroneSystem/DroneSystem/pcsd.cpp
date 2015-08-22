#include <pcsd.h>
#include <iostream>

DroneSystem::DroneSystem(int ID, int SUPort, NetAddress Syn, QObject *parent) : QObject(parent), m_ID(ID), m_SynF(Syn)
{
    m_SynUdpPort = SUPort;
    m_SynUdpSocket = new QUdpSocket(this);
    m_SynUdpSocket->bind(QHostAddress::Any, m_SynUdpPort);
    connect(m_SynUdpSocket, SIGNAL(readyRead()), this, SLOT(ReadFromSynUDP()));
    std::cout << m_SynF.IP.toString().toStdString() << " " << m_SynF.PORT << std::endl;
}

void DroneSystem::ReadFromSynUDP()
{
    while(m_SynUdpSocket->hasPendingDatagrams())
    {

        QByteArray buffer;
        buffer.resize(m_SynUdpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        m_SynUdpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        std::string tempStr = buffer.toStdString();
        std::map<int, Address> tempSpace = StrToData(tempStr);

        if(!IsEqualState(tempSpace))
        {
            m_DronesSpace = tempSpace;
            std::cout << "State changed...\nNow we have " << m_DronesSpace.size() << "\n" << tempStr << std::endl;
        }
    }
}

bool DroneSystem::IsEqualState(std::map<int, Address> &state)
{
    std::string strSt1 = DataToStr(state);
    std::string strSt2 = DataToStr(m_DronesSpace);

    if(strSt1.size() != strSt2.size())
        return false;

    if(strSt1.compare(strSt2) == 0)
        return true;
    return false;
}

void DroneSystem::Work()
{
    while(1)
    {
        delay(1000);
        DoSync();
    }
}

void DroneSystem::DoSync()
{
   SendIDToSync();
}

void DroneSystem::SendIDToSync()
{
    QByteArray Data;
    Data.append(QString::number(m_ID));
    m_SynUdpSocket->writeDatagram(Data, m_SynF.IP, m_SynF.PORT);
}
