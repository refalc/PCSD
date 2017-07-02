#include "cnetworkconnectionfactory.h"
#include "networkconnection.h"

CNetworkConnectionFactory::CNetworkConnectionFactory()
{
    m_iBuildedCounter = 0;
}

std::unique_ptr<INetworkConnectionFactory> CNetworkConnectionFactory::m_pInstance;
INetworkConnectionFactory* CNetworkConnectionFactory::Instance()
{
    if( !m_pInstance ) {
        m_pInstance.reset(new CNetworkConnectionFactory());
    }
    return m_pInstance.get();
}

std::unique_ptr<INetworkConnection> CNetworkConnectionFactory::CreateConnection(INetworkObject *network_object, const SocketType type, const int port) const
{
    switch( type ) {
    case SocketType::UDP:
        return std::unique_ptr<INetworkConnection>(new CNetworkConnectionUDP(network_object, port, m_iBuildedCounter++));
    case SocketType::TCP:
        return std::unique_ptr<INetworkConnection>(new CNetworkConnectionTCP(network_object, port, m_iBuildedCounter++));
    }

    return nullptr;
}
