#include "testconnection.h"
#include "../Utils/Utils/cnetworkconnectionfactory.h"

CTestConnectionReciever::CTestConnectionReciever(int cur_port_1, int cur_port_2, int cur_port_tcp)
{
    m_NetworkConnectionUDP_1 = CNetworkConnectionFactory::Instance()->CreateConnection(this, SocketType::UDP, cur_port_1);
    m_NetworkConnectionUDP_2 = CNetworkConnectionFactory::Instance()->CreateConnection(this, SocketType::UDP, cur_port_2);
    m_NetworkConnectionTCPServer = CNetworkConnectionFactory::Instance()->CreateConnection(this, SocketType::TCP_Server, cur_port_tcp);
}

bool CTestConnectionReciever::TakeDataFromQuery(std::string &str_data, Address &from, int &connection)
{
    str_data.clear();
    if( !m_Query.empty() ) {
        NetworkDataFromNS data = m_Query.front();
        m_Query.pop();

        data.second.GetData(str_data);
        data.second.GetReceiver(from);
        connection = data.first;
    } else
        return false;

    return true;
}

void CTestConnectionReciever::Work()
{
    CNetworkData ok;
    ok.SetData(std::string("OK"));

    int counter = 0;
    std::string data;
    m_NetworkConnectionTCPServer->Listen();
    while(1)
    {
        if(counter == 10)
            counter = 0;
        delay(100);
        Address from;
        int connection = -1;
        if( TakeDataFromQuery(data, from, connection) ) {
            std::cout << "Get Data : " << data << std::endl;
            std::cout << "\tFrom : " << from.m_IP << "|" << std::to_string(from.m_iPort) << std::endl;
            std::cout << "\tBy connection #" << std::to_string(connection) << std::endl;
            ok.SetReceiver(from);
            if( m_NetworkConnectionUDP_1->GetConnectionID() == connection )
                m_NetworkConnectionUDP_1->Send(ok);
            if( m_NetworkConnectionUDP_2->GetConnectionID() == connection )
                m_NetworkConnectionUDP_2->Send(ok);
            if( m_NetworkConnectionTCPServer->GetConnectionID() == connection )
                m_NetworkConnectionTCPServer->Send(ok);
        }


        counter++;
    }
}


CTestConnectionSender::CTestConnectionSender(SocketType type, int cur_port, const Address &reciever)
{
    m_Friend = reciever;
    m_NetworkConnection = CNetworkConnectionFactory::Instance()->CreateConnection(this, type, cur_port);
}

void CTestConnectionSender::Work()
{
    CNetworkData hello;
    hello.SetData(std::string("Hello from sender\n"));
    hello.SetReceiver(m_Friend);

    int counter = 0, connec;
    std::string data;
    bool send_ok = true;
    while( m_NetworkConnection->ConnectTo(m_Friend, 3000) != ErrCode::No_Error )
        std::cout << "Fail to Connect\n";
    while(1)
    {
        if(counter == 10)
            counter = 0;
        delay(1000);
        if( send_ok ) {
            m_NetworkConnection->Send(hello);
            send_ok = false;
        }
        Address temp;
        if( TakeDataFromQuery(data, temp, connec) ) {
            if( !data.compare("OK") ) {
                std::cout << "Get OK from " << temp.m_IP << " " << temp.m_iPort << std::endl;
                send_ok = true;
            } else {
                std::cout << "Take incorrect command " << data << std::endl;
            }
        }

        counter++;
    }
}

bool CTestConnectionSender::TakeDataFromQuery(std::string &str_data, Address &from, int &connection)
{
    str_data.clear();
    if( !m_Query.empty() ) {
        NetworkDataFromNS data = m_Query.front();
        m_Query.pop();

        data.second.GetData(str_data);
        data.second.GetReceiver(from);
        connection = data.first;
    } else
        return false;

    return true;
}
