#include "utils.h"
#include <cstdlib>

std::string Func(int i)
{
    char buff[6];
    std::string res;
    itoa(i, buff, 10);
    res.append(buff);
    return res;
}

std::string DataToStr(std::map<int, Address> &data)
{
    char buff[6];
    std::string res;
    for(auto it = data.begin(), it_end = data.end(); it != it_end; it++)
    {
        itoa(it->second.ID, buff, 10);
        res.append(buff);
        res.append(" ");
        res.append(it->second.IP);
        res.append(" ");
        itoa(it->second.PORT, buff, 10);
        res.append(buff);
        res.append("|");
    }
    return res;
}

std::map<int, Address> StrToData(std::string &str)
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
                strID.clear();
                tempData.ID = atoi(strID.c_str());
            }
            else if(state_counter == 1)
            {
                tempData.IP = IP;
                IP.clear();
            }
            else if(state_counter == 2)
            {
                tempData.PORT = atoi(strPORT.c_str());
                strPORT.clear();
            }
            state_counter++;
            if(state_counter == 3)
            {
                state_counter = 0;
                data[tempData.ID] = tempData;
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










