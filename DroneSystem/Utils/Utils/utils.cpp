#include "utils.h"
#include <cstdlib>
#include <QTime>
#include <QCoreApplication>
#include <iostream>
#include <sstream>

void delay(int m)
{
    QTime dieTime= QTime::currentTime().addMSecs(m);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
std::string DataToStr(const std::map<int, Address> &data)
{
    char buff[6];
    std::string res;
    for(auto it = data.begin(), it_end = data.end(); it != it_end; it++)
    {
        itoa(it->second.m_iID, buff, 10);
        res.append(buff);
        res.append(" ");
        res.append(it->second.m_IP);
        res.append(" ");
        itoa(it->second.m_iPort, buff, 10);
        res.append(buff);
        res.append("|");
    }
    return res;
}

std::map<int, Address> StrToData(const std::string &str)
{
    std::map<int, Address> data;
    Address tempData;
    std::string IP, strID, strPORT;
    int state_counter = 0;
    for(auto it = str.begin(), it_end = str.end(); it != it_end; it++)
    {
        if(*it == ' ' || *it == '|')
        {
            if(state_counter == 0)
            {
                tempData.m_iID = atoi(strID.c_str());
                strID.clear();
            }
            else if(state_counter == 1)
            {
                tempData.m_IP = IP;
                IP.clear();
            }
            else if(state_counter == 2)
            {
                tempData.m_iPort = atoi(strPORT.c_str());
                strPORT.clear();
            }
            state_counter++;
            if(state_counter == 3)
            {
                state_counter = 0;
                data[tempData.m_iID] = tempData;
            }
        }
        else if(state_counter == 0)
        {
            strID.push_back(*it);
        }
        else if(state_counter == 1)
        {
            IP.push_back(*it);
        }
        else if(state_counter == 2)
        {
            strPORT.push_back(*it);
        }
    }
    return data;
}


void split_string_by_delim(const std::string &str, char delim, std::vector<std::string> &result) {
    std::stringstream str_stream;
    str_stream.str(str);
    std::string item;
    while( std::getline(str_stream, item, delim) )
        result.emplace_back(std::move(item));
}

void CNetworkData::SetReceiver(const Address &receiver)
{
    m_Receiver = receiver;
}

void CNetworkData::GetData(std::string &data) const
{
    data = m_Data;
}

void CNetworkData::GetReceiver(Address &receiver) const
{
    receiver = m_Receiver;
}

std::string CNetworkData::ToString() const
{
    return m_Data;
}








