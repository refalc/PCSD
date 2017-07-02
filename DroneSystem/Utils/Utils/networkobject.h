#pragma once
#include "utils.h"


class CNetworkObject : public INetworkObject
{
public:
    virtual void AddPendingDataFromNC(NetworkDataFromNS &&data) override;

private:
    std::queue<NetworkDataFromNS> m_Query;
};
