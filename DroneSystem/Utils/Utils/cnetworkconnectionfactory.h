#pragma once
#include "utils.h"

class CNetworkConnectionFactory : public INetworkConnectionFactory
{
public:
    static INetworkConnectionFactory * Instance();
    virtual std::unique_ptr<INetworkConnection> CreateConnection(INetworkObject *network_object, const SocketType type, const int port) const override;

private:
    CNetworkConnectionFactory();
    static std::unique_ptr<INetworkConnectionFactory> m_pInstance;
    mutable int m_iBuildedCounter;
};
