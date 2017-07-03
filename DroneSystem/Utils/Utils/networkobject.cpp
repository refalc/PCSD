#include "networkobject.h"
#include <iostream>
void CNetworkObject::AddPendingDataFromNC(NetworkDataFromNS &&data)
{
    m_Query.push(std::move(data));
}
