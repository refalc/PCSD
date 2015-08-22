#include <pcsd.h>
#include <iostream>
#include <algorithm>

DroneSystem::DroneSystem(int ID, int SUPort, Address &Syn, QObject *parent) : QObject(parent), m_ID(ID), m_SynF(Syn)
{
    m_isCoordinator = false;
    m_Alone = true;

    m_SynUdpPort = SUPort;
    m_SynUdpSocket = new QUdpSocket(this);
    m_SynUdpSocket->bind(QHostAddress::Any, m_SynUdpPort);
    connect(m_SynUdpSocket, SIGNAL(readyRead()), this, SLOT(ReadFromSynUDP()));

    m_CrossDroneUdpPort = m_SynUdpPort + 1;
    m_CrossDroneUdpSocket = new QUdpSocket(this);
    m_CrossDroneUdpSocket->bind(QHostAddress::Any, m_CrossDroneUdpPort);
    connect(m_CrossDroneUdpSocket, SIGNAL(readyRead()), this, SLOT(ReadFromLastDrone()));

    std::cout << m_SynF.IP << " " << m_SynF.PORT << std::endl;
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
                    m_NextDrone.ID = m_DronesSpace[*it].ID;
                    m_NextDrone.IP = m_DronesSpace[*it].IP;
                    m_NextDrone.PORT = m_DronesSpace[*it].PORT;
                    it--;
                }
                else
                {
                    m_NextDrone.ID = m_DronesSpace[m_DroneIDs[0]].ID;
                    m_NextDrone.IP = m_DronesSpace[m_DroneIDs[0]].IP;
                    m_NextDrone.PORT = m_DronesSpace[m_DroneIDs[0]].PORT;
                }

                //Last Drone Address
                if(it != m_DroneIDs.begin())
                {
                    it--;
                    m_LastDrone.ID = m_DronesSpace[*it].ID;
                    m_LastDrone.IP = m_DronesSpace[*it].IP;
                    m_LastDrone.PORT = m_DronesSpace[*it].PORT;
                    it++;
                }
                else
                {
                    m_LastDrone.ID = m_DronesSpace[m_DroneIDs[m_DroneIDs.size()-1]].ID;
                    m_LastDrone.IP = m_DronesSpace[m_DroneIDs[m_DroneIDs.size()-1]].IP;
                    m_LastDrone.PORT = m_DronesSpace[m_DroneIDs[m_DroneIDs.size()-1]].PORT;
                }

                std::cout << "My Last NB have ID " << m_LastDrone.ID << "\nMy Next NB have ID " << m_NextDrone.ID << std::endl;

            }
            std::cout << "State changed...\nNow we have " << m_DronesSpace.size() << "\n" << tempStr << std::endl;
        }
    }
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

        if(QHostAddress(sender.toString().toStdString().substr(7).c_str()) == QHostAddress(m_LastDrone.IP.c_str()) && senderPort == m_LastDrone.PORT)
        {
            DecodeTask(buffer.toStdString());
            if(!m_isCoordinator)
                SendNext(buffer);
        }
        else
        {
            std::cout << "Error: another sender:\n   Last Drone IP = " << m_LastDrone.IP <<"\n   Last Drone PORT = " << m_LastDrone.PORT
                      << "\n   Sender IP = " << sender.toString().toStdString() << "\n   Sender PORT = " << senderPort << std::endl;
        }

    }
}

void DroneSystem::DecodeTask(std::string inputData)
{
    std::string strID, strTask;
    int Task, ID;
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
                Task = atoi(strTask.c_str());
                m_Tasks.push_back(Task);
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

void DroneSystem::SendNext(QByteArray &data)
{
    m_CrossDroneUdpSocket->writeDatagram(data, QHostAddress(m_NextDrone.IP.c_str()), m_NextDrone.PORT);
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
    bool TestTask = true;
    while(1)
    {
        delay(1000);
        if(m_isCoordinator && TestTask)
        {
            //generate test task iteration
            Iteration task_iter;
            for(int i = 0; i < m_DroneIDs.size(); i++)
            {
                std::pair<int, int> temp(m_DroneIDs[i], m_ID);
                task_iter.push_back(temp);
            }
            m_AllCTasks.push_back(task_iter);
            //

            DoTaskIteration();
            TestTask  = false;
        }
        DoSync();
    }
}

void DroneSystem::DoTaskIteration()
{
    Iteration currIter = m_AllCTasks[0];

    std::string strData;
    char buffer[10];
    int ISize = currIter.size();

    for(int i = 0; i < ISize; i++)
    {
        itoa(currIter[i].first, buffer, 10);
        strData.append(buffer);
        strData.push_back(' ');
        itoa(currIter[i].second, buffer, 10);
        strData.append(buffer);
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
    m_SynUdpSocket->writeDatagram(Data,QHostAddress(m_SynF.IP.c_str()), m_SynF.PORT);
}
