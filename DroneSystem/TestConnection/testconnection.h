#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <iostream>
#include "../Utils/Utils/utils.h"
#include "../Utils/Utils/networkobject.h"

class CTestConnectionReciever : public CNetworkObject
{
public:
    CTestConnection(int cur_port_1, int cur_port_2);
    void Work();

private:
    bool TakeDataFromQuery(std::string &data);

private:
    int m_iID;
    int m_Counter;
    std::unique_ptr<INetworkConnection> m_NetworkConnectionUDP_1;
    std::unique_ptr<INetworkConnection> m_NetworkConnectionUDP_2;
};


#endif // TESTCONNECTION_H

