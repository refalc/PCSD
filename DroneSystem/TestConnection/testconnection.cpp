#include "testconnection.h"
#include "../Utils/Utils/cnetworkconnectionfactory.h"

CTestConnectionReciever::CTestConnectionReciever(int cur_port_1, int cur_port_2)
{
    m_NetworkConnectionUDP_1 = CNetworkConnectionFactory::Instance()->CreateConnection(this, SocketType::UDP, cur_port_1);
    m_NetworkConnectionUDP_2 = CNetworkConnectionFactory::Instance()->CreateConnection(this, SocketType::UDP, cur_port_2);
}

bool CTestConnectionReciever::TakeDataFromQuery(std::string &data, Address &from)
{
    data.clear();
    if( !m_Query.empty() ) {
        NetworkDataFromNS data = m_Query.front();
        m_Query.pop();

        data.second.GetData(data);
        data.second.GetReceiver(from);
    } else
        return false;

    return true;
}

void CTestConnection::Work()
{
    CNetworkData hello, ok;
    hello.SetData(std::string("Hello from " + std::to_string(cur_port)));
    hello.SetReceiver(m_Friend);

    ok.SetData(std::string("OK from " + std::to_string(cur_port)));
    ok.SetReceiver(m_Friend);

    int counter = 0;
    bool first_msg = true;
    int wait_for_ok = 0;
    std::string data;
    while(1)
    {
        if(counter == 10)
            counter = 0;
        delay(1000);

        if( first_msg ) {
            m_NetworkConnectionUDP->Send(hello);
            first_msg = false;
            wait_for_ok = 0;
        }

        // check for OK
        if( TakeDataFromQuery(data) ) {

        }


        counter++;
    }
}
