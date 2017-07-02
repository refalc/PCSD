#include "networkobject.h"

void CNetworkObject::AddPendingDataFromNC(NetworkDataFromNS &&data)
{
    m_Querry.push(std::move(data));
}
