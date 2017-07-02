#include <pcsd.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

DroneSystem::DroneSystem(int ID, int SUPort, int TCPPort, const Address &Syn, QObject *parent) : QObject(parent), m_ID(ID), m_SynF(Syn)
{
    m_isCoordinator = false;
    m_Alone = true;
    m_OkTask = true;

    m_SynUdpPort = SUPort;
    m_SynUdpSocket = new QUdpSocket(this);
    m_SynUdpSocket->bind(QHostAddress::Any, m_SynUdpPort);
    connect(m_SynUdpSocket, SIGNAL(readyRead()), this, SLOT(ReadFromSynUDP()));

    m_CrossDroneUdpPort = m_SynUdpPort + 1;
    m_CrossDroneUdpSocket = new QUdpSocket(this);
    m_CrossDroneUdpSocket->bind(QHostAddress::Any, m_CrossDroneUdpPort);
    connect(m_CrossDroneUdpSocket, SIGNAL(readyRead()), this, SLOT(ReadFromLastDrone()));

    m_TcpPort = TCPPort;
    m_TcpSocket = new QTcpSocket(this);
    connect(m_TcpSocket, SIGNAL(connected()),this, SLOT(ConnectedCube()));
    connect(m_TcpSocket, SIGNAL(disconnected()),this, SLOT(DisconnectedCube()));
    connect(m_TcpSocket, SIGNAL(readyRead()),this, SLOT(ReadCube()));

    std::cout << "connecting to TCP...\n";

    m_TcpSocket->connectToHost("127.0.0.1", m_TcpPort);

    if(!m_TcpSocket->waitForConnected(2000))
    {
        std::cout << "Error: " << m_TcpSocket->errorString().toStdString() << std::endl;
    }

    std::cout << m_SynF.m_IP << " " << m_SynF.m_iPort << std::endl;
}

void DroneSystem::ConnectedCube()
{
    std::cout << "Connected\n";
    SendCube("G_CRD|");
}

void DroneSystem::DisconnectedCube()
{
    std::cout << "Disonnected\n";
}

void DroneSystem::ReadCube()
{
    //todo commands
    DecodeCommand(m_TcpSocket->readAll().toStdString());
}

void DroneSystem::DecodeCommand(const std::string &command)
{
    std::cout << "Decode Command: " << command << std::endl;
    if(command.find("CRD_") != command.npos)
    {
        int pos = command.find("_");
        pos++;
        std::string strCrd;
        int state = 0;
        while(command[pos] != '|')
        {
            if(command[pos] == ':')
            {
                m_Crd[state] = atof(strCrd.c_str());
                strCrd.clear();
                state++;
            }
            else
            {
                strCrd.push_back(command[pos]);
            }
            pos++;
        }
        m_Crd[state] = atof(strCrd.c_str());
    }
    else if(command.find("OK") != command.npos)
    {
        std::cout << "recive OK\n";
        m_OkTask = true;
    }
    else
    {
        std::cout << "Recive wrong command: " << command << std::endl;
    }
}

void DroneSystem::SendCube(const std::string &command)
{
    m_TcpSocket->write(command.c_str());
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

        if(buffer.toStdString().find("M_") != std::string::npos)
        {
            MagicFunction(buffer.toStdString());
            continue;
        }

        std::string tempStr = buffer.toStdString();
        std::map<int, Address> tempSpace = StrToData(tempStr);

        if(!IsEqualState(tempSpace))
        {
            m_DronesSpace = tempSpace;

            if(m_Alone)
                if(m_DronesSpace.size() > 1)
                    m_Alone = false;

            auto last_space_it = m_DronesSpace.end();
            last_space_it--;
            if(m_DronesSpace.size() + 1 == m_DroneIDs.size())
                m_DroneIDs.push_back(last_space_it->first);
            else
            {
                m_DroneIDs.clear();
                for(auto space_it = m_DronesSpace.begin(); space_it != m_DronesSpace.end(); space_it++)
                     m_DroneIDs.push_back(space_it->first);
            }
            std::sort(m_DroneIDs.begin(), m_DroneIDs.end());

            auto last_it = m_DroneIDs.end();
            last_it--;
            auto it = find(m_DroneIDs.begin(), m_DroneIDs.end(), m_ID);

            if(it == last_it && !m_isCoordinator)
            {
                m_isCoordinator = true;
                std::cout << "I am COORDINATOR now\n";
            }
            else if(it != last_it && m_isCoordinator)
            {
                m_isCoordinator = false;
                std::cout << "I am not COORDINATOR now T_T, but ID = " << *last_it <<"\n";
            }

            if(!m_Alone)
            {
                //Next Drone Address
                if(!m_isCoordinator)
                {
                    it++;
                    m_NextDrone.m_iID = m_DronesSpace[*it].m_iID;
                    m_NextDrone.m_IP = m_DronesSpace[*it].m_IP;
                    m_NextDrone.m_iPort = m_DronesSpace[*it].m_iPort;
                    it--;
                }
                else
                {
                    m_NextDrone.m_iID = m_DronesSpace[m_DroneIDs[0]].m_iID;
                    m_NextDrone.m_IP = m_DronesSpace[m_DroneIDs[0]].m_IP;
                    m_NextDrone.m_iPort = m_DronesSpace[m_DroneIDs[0]].m_iPort;
                }

                //Last Drone Address
                if(it != m_DroneIDs.begin())
                {
                    it--;
                    m_LastDrone.m_iID = m_DronesSpace[*it].m_iID;
                    m_LastDrone.m_IP = m_DronesSpace[*it].m_IP;
                    m_LastDrone.m_iPort = m_DronesSpace[*it].m_iPort;
                    it++;
                }
                else
                {
                    m_LastDrone.m_iID = m_DronesSpace[m_DroneIDs[m_DroneIDs.size()-1]].m_iID;
                    m_LastDrone.m_IP = m_DronesSpace[m_DroneIDs[m_DroneIDs.size()-1]].m_IP;
                    m_LastDrone.m_iPort = m_DronesSpace[m_DroneIDs[m_DroneIDs.size()-1]].m_iPort;
                }

                std::cout << "My Last NB have ID " << m_LastDrone.m_iID << "\nMy Next NB have ID " << m_NextDrone.m_iID << std::endl;

            }
            std::cout << "State changed...\nNow we have " << m_DronesSpace.size() << "\n" << tempStr << std::endl;
        }
    }
}

void DroneSystem::MagicFunction(const std::string &command)
{
    //need algorithm
    std::cout << "Magic Function get command: " << command << std::endl;
    Iteration tempIteration;

    std::random_device rd;
    std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> dist(-7, 7);
    std::uniform_real_distribution<double> u_dist(0, 15);

    std::string r_command;
    double x, y, z;
    for(auto it = m_DroneIDs.begin(); it != m_DroneIDs.end(); it++)
    {
        x = dist(mt);
        y = dist(mt);
        z = u_dist(mt);

        r_command.append("M_");
        r_command.append(std::to_string(x));
        r_command.append(":");
        r_command.append(std::to_string(y));
        r_command.append(":");
        r_command.append(std::to_string(z));
        r_command.append("|");

        tempIteration.push_back(std::pair<int, std::string>(*it, r_command));
        r_command.clear();
    }
    m_AllCTasks.push_back(tempIteration);
}

void DroneSystem::ReadFromLastDrone()
{
    //todo add type of data
    while(m_CrossDroneUdpSocket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(m_CrossDroneUdpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        m_CrossDroneUdpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        if(QHostAddress(sender.toString().toStdString().substr(7).c_str()) == QHostAddress(m_LastDrone.m_IP.c_str()) && senderPort == m_LastDrone.m_iPort)
        {
            DecodeTask(buffer.toStdString());
            if(!m_isCoordinator)
                SendNext(buffer);
        }
        else
        {
            std::cout << "Error: another sender:\n   Last Drone IP = " << m_LastDrone.m_IP <<"\n   Last Drone PORT = " << m_LastDrone.m_iPort
                      << "\n   Sender IP = " << sender.toString().toStdString() << "\n   Sender PORT = " << senderPort << std::endl;
        }

    }
}

void DroneSystem::DecodeTask(const std::string &inputData)
{
    std::string strID, strTask;
    int ID;
    int state = 0;
    std::cout << "I Decode: " <<  inputData << "\n";
    for(auto it = inputData.begin(), it_end = inputData.end(); it != it_end; it++)
    {
        if(state == 0)
        {
            if(*it == ' ')
            {
                ID = atoi(strID.c_str());
                strID.clear();
                state = 1;
            }
            else
                strID.push_back(*it);
        }
        else if(ID == m_ID)
        {
            if(*it == ' ' || *it == '|')
            {
                strTask.append("|");
                m_Tasks.push_back(strTask);
                break;
            }
            else
                strTask.push_back(*it);
        }
        else if(*it == ' ')
            state = 0;

    }

    if(m_Tasks.size() == 0)
        std::cout << "I have not any task\n";
    else
    {
        std::cout << "I have task's:\n   ";
        for(int i = 0; i < m_Tasks.size(); i++)
        {
            std::cout << m_Tasks[i] << " ";
        }
        std::cout << std::endl;
    }
}

void DroneSystem::SendNext(const QByteArray &data)
{
    m_CrossDroneUdpSocket->writeDatagram(data, QHostAddress(m_NextDrone.m_IP.c_str()), m_NextDrone.m_iPort);
}

bool DroneSystem::IsEqualState(const std::map<int, Address> &state) const
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
    DecodeCommand("CRD_0.1:0:2|");
    std::cout << "My Crd: X = " << m_Crd[0] << " Y = " << m_Crd[1] << " Z = " << m_Crd[2] << std::endl;

    int counter = 0;
    while(1)
    {
        if(counter == 10)
            counter = 0;
        delay(1000);
        if(m_isCoordinator)
        {
            if(m_AllCTasks.size() != 0)
                DoTaskIteration();
            //generate test task iteration
        }
        if(counter == 0)
            DoSync();
        DoTask();
        counter++;
    }
}

void DroneSystem::DoTask()
{
    if(m_OkTask && m_Tasks.size() != 0)
    {
        SendCube(m_Tasks[0]);
        m_Tasks.pop_front();
        m_OkTask = false;
    }
}

void DroneSystem::DoTaskIteration()
{
    std::cout << "Coordinator say WORK\n";
    Iteration currIter = m_AllCTasks[0];
    m_AllCTasks.pop_front();

    std::string strData;
    char buffer[10];
    int ISize = currIter.size();

    for(int i = 0; i < ISize; i++)
    {
        itoa(currIter[i].first, buffer, 10);
        strData.append(buffer);
        strData.push_back(' ');
        strData.append(currIter[i].second);
        strData.push_back(' ');
    }
    strData[strData.size()-1] = '|';
    QByteArray data(strData.c_str());
    SendNext(data);
}

void DroneSystem::DoSync()
{
   SendIDToSync();
}

void DroneSystem::SendIDToSync()
{
    QByteArray Data;
    Data.append(QString::number(m_ID));
    Data.append(" ");
    Data.append(QString::number(m_CrossDroneUdpPort));
    m_SynUdpSocket->writeDatagram(Data,QHostAddress(m_SynF.m_IP.c_str()), m_SynF.m_iPort);
}
