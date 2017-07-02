#include "networkconnection.h"
#include <iostream>

CNetworkConnection::CNetworkConnection(INetworkObject *network_object, const int port, const int id)
{
    m_ConnectedNetworkObject = network_object;
    m_iPort = port;
    m_iID = id;
}

ErrCode CNetworkConnection::Send(const CNetworkData &data)
{
    std::cout << "Not Implemented\n";

    return ErrCode::General_Error;
}

CNetworkConnectionUDP::CNetworkConnectionUDP(INetworkObject *network_object, const int port, const int id) : CNetworkConnection(network_object, port, id)
{
    // todo some check ?
    m_UDPSocket = new QUdpSocket(this);
    m_UDPSocket->bind(QHostAddress::Any, m_iPort);
    connect(m_UDPSocket, SIGNAL(readyRead()), this, SLOT(ReadUDP()));
}

CNetworkConnectionUDP::~CNetworkConnectionUDP()
{
    delete m_UDPSocket;
}

ErrCode CNetworkConnectionUDP::Send(const CNetworkData &data)
{
    Address addr;
    data.GetReceiver(addr);
    QByteArray byte_array_data(data.ToString().c_str());
    m_UDPSocket->writeDatagram(byte_array_data, QHostAddress(addr.m_IP.c_str()), addr.m_iPort);

    return ErrCode::No_Error;
}

void CNetworkConnectionUDP::ReadUDP()
{
    while(m_UDPSocket->hasPendingDatagrams()) {
        QByteArray recieved_data;
        recieved_data.resize(m_UDPSocket->pendingDatagramSize());

        QHostAddress sender_address;
        quint16 sender_port;
        m_UDPSocket->readDatagram(recieved_data.data(), recieved_data.size(), &sender_address, &sender_port);

        // todo : create data from recieved_data + sender + port + id
        NetworkDataFromNS data;
        data.first = m_iID;
        data.second.SetData(recieved_data.toStdString());
        data.second.SetReceiver(Address());
        m_ConnectedNetworkObject->AddPendingDataFromNC(std::move(data));
    }
}

CNetworkConnectionTCP::CNetworkConnectionTCP(INetworkObject *network_object, const int port, const int id) : CNetworkConnection(network_object, port, id)
{
    m_TCPSocket = new QTcpSocket(this);
    connect(m_TCPSocket, SIGNAL(connected()),this, SLOT(ConnectedTCP()));
    connect(m_TCPSocket, SIGNAL(disconnected()),this, SLOT(DisconnecteTCP()));
    connect(m_TCPSocket, SIGNAL(readyRead()),this, SLOT(ReadTCP()));
    m_bConnected = false;
}

CNetworkConnectionTCP::~CNetworkConnectionTCP()
{
    delete m_TCPSocket;
}

ErrCode CNetworkConnectionTCP::Send(const CNetworkData &data)
{
    if( m_bConnected )
        m_TCPSocket->write(data.ToString().c_str());
    else
        return ErrCode::Network_Error;

    return ErrCode::No_Error;
}

void CNetworkConnectionTCP::ConnectedTCP()
{
    m_bConnected = true;
}

void CNetworkConnectionTCP::DisconnectedTCP()
{
    m_bConnected = false;
}

void CNetworkConnectionTCP::ReadTCP()
{
    std::string input_data = m_TCPSocket->readAll().toStdString();
    NetworkDataFromNS data;
    Address addr;

    data.second.SetData(std::move(input_data));

    addr.m_iID = -1;
    addr.m_IP = m_TCPSocket->peerAddress().toString().toStdString();
    addr.m_iPort = m_TCPSocket->peerPort();

    data.first = m_iID;
    data.second.SetReceiver(addr);
    m_ConnectedNetworkObject->AddPendingDataFromNC(std::move(data));
}




