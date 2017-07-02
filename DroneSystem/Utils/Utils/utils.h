#pragma once

#include <map>
#include <vector>
#include <utility>
#include <string>
#include <queue>
#include <memory>
#include <qobject.h>

// predef
class CNetworkData;
struct Address;
//

// typedef
typedef std::vector<std::pair<int, std::string> > Iteration;
typedef std::pair<int, CNetworkData> NetworkDataFromNS;
//

// enums
enum class SocketType
{
    UDP = 0,
    TCP = 1,
};

enum class ErrCode
{
    No_Error = 0,
    General_Error = 1,
    Network_Error = 2,
};

//

// functions
std::string DataToStr(const std::map<int, Address> & data);
std::map<int, Address> StrToData(const std::string &str);
void delay(int m);
void split_string_by_delim(const std::string &str, char delim, std::vector<std::string> &result);
//

// classes
struct Address
{
    Address() : m_iID(0), m_IP(""), m_iPort(0){}
    Address(int id, const std::string &ip, int port) : m_iID(id), m_IP(""), m_iPort(0){}
    int m_iID;
    std::string m_IP;
    int m_iPort;
};

class CNetworkData
{
public:
    template<typename T>
    void SetData(T &&data)
    {
        m_Data = std::forward<T>(data);
    }
    void SetReceiver(const Address &receiver);
    void GetData(std::string &data) const;
    void GetReceiver(Address &receiver) const;
    std::string ToString() const;

private:
    std::string m_Data;
    Address m_Receiver;
};

class INetworkObject
{
public:
    virtual void AddPendingDataFromNC(NetworkDataFromNS &&data) = 0;
};


class INetworkConnection
{
public:
    virtual ErrCode Send(const CNetworkData &data) = 0;
};

class INetworkConnectionFactory
{
public:
    virtual std::unique_ptr<INetworkConnection> CreateConnection(INetworkObject *network_object, const SocketType type, const int port) const = 0;
};

// QT MAGIC
Q_DECLARE_INTERFACE(INetworkConnection, "INetworkConnection/1.0")
