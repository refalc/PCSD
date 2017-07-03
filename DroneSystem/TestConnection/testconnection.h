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
    CTestConnectionReciever(int cur_port_1, int cur_port_2, int cur_port_tcp);
    void Work();

private:
    bool TakeDataFromQuery(std::string &str_data, Address &from, int &connection);

private:
    int m_iID;
    int m_Counter;
    std::unique_ptr<INetworkConnection> m_NetworkConnectionUDP_1;
    std::unique_ptr<INetworkConnection> m_NetworkConnectionUDP_2;
    std::unique_ptr<INetworkConnection> m_NetworkConnectionTCPServer;
};

class CTestConnectionSender : public CNetworkObject
{
public:
    CTestConnectionSender(SocketType type, int cur_port, const Address &reciever);
    void Work();

private:
    bool TakeDataFromQuery(std::string &str_data, Address &from, int &connection);

private:
    Address m_Friend;
    int m_iID;
    int m_Counter;
    std::unique_ptr<INetworkConnection> m_NetworkConnection;
    //std::unique_ptr<INetworkConnection> m_NetworkConnection;
};


#endif // TESTCONNECTION_H

