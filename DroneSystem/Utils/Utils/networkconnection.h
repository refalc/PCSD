#pragma once
#include "utils.h"
#include <QUdpSocket>
#include <QTcpSocket>

class CNetworkConnection : public QObject, public INetworkConnection
{
    Q_OBJECT
    Q_INTERFACES(INetworkConnection)
public:
    CNetworkConnection(INetworkObject *network_object, const int port, const int id);
    virtual ErrCode Send(const CNetworkData &data) override;

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
    void ReadUDP();

private:
    QUdpSocket *m_UDPSocket;
};

class CNetworkConnectionTCP : public CNetworkConnection
{
    Q_OBJECT
public:
    CNetworkConnectionTCP(INetworkObject *network_object, const int port, const int id);
    ~CNetworkConnectionTCP();
    virtual ErrCode Send(const CNetworkData &data) override;

signals:

public slots:
    void ConnectedTCP();
    void DisconnectedTCP();
    void ReadTCP();

private:
    QTcpSocket *m_TCPSocket;
    int m_iTCPPort;
    INetworkObject *m_ConnectedNetworkObject;
    bool m_bConnected;
};



