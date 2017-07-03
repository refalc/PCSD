#pragma once
#include "utils.h"
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>

class CNetworkConnection : public QObject, public INetworkConnection
{
    Q_OBJECT
    Q_INTERFACES(INetworkConnection)
public:
    CNetworkConnection(INetworkObject *network_object, const int port, const int id);
    virtual ErrCode Send(const CNetworkData &data) override;
    virtual int GetConnectionID() override final;

    //tsp
    virtual ErrCode Listen(const std::string &from = std::string("")) override;
    virtual ErrCode ConnectTo(const Address &address, long wait_millsec) override;

protected:
    int m_iID;
    int m_iPort;
    INetworkObject *m_ConnectedNetworkObject;
};

class CNetworkConnectionUDP : public CNetworkConnection
{
    Q_OBJECT
public:
    CNetworkConnectionUDP(INetworkObject *network_object, const int port, const int id);
    ~CNetworkConnectionUDP();
    virtual ErrCode Send(const CNetworkData &data) override final;

signals:

public slots:
    void Read();

private:
    QUdpSocket *m_UDPSocket;
};

class CNetworkConnectionTCP : public CNetworkConnection
{
    Q_OBJECT
public:
    CNetworkConnectionTCP(INetworkObject *network_object, const int port, const int id);
    ~CNetworkConnectionTCP();
    virtual ErrCode Send(const CNetworkData &data) override final;
    virtual ErrCode ConnectTo(const Address &address, long wait_millsec) override;
    virtual ErrCode Listen(const std::string &from = std::string("")) override;

public slots:
    void Read();
    virtual void Connected();
    virtual void Disconnected();

protected:
    QTcpSocket *m_TCPSocket;
    bool m_bConnected;
};

class CNetworkConnectionTCPClient : public CNetworkConnectionTCP
{
    Q_OBJECT
public:
    CNetworkConnectionTCPClient(INetworkObject *network_object, const int port, const int id);
    ~CNetworkConnectionTCPClient();
    virtual ErrCode ConnectTo(const Address &address, long wait_millsec) override final;

signals:

public slots:
    virtual void Connected() override;
    virtual void Disconnected() override;
};

class CNetworkConnectionTCPServer : public CNetworkConnectionTCP
{
    Q_OBJECT
public:
    CNetworkConnectionTCPServer(INetworkObject *network_object, const int port, const int id);
    ~CNetworkConnectionTCPServer();
    virtual ErrCode Listen(const std::string &from = std::string("")) override final;

signals:

public slots:
    virtual void Connected() override;
    virtual void Disconnected() override;

private:
    QTcpServer *m_TCPServer;
};

