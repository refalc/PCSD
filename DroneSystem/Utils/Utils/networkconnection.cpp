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

int CNetworkConnection::GetConnectionID()
{
    return m_iID;
}

ErrCode CNetworkConnection::Listen(const std::string &from)
{
    std::cout << "Not Implemented\n";

    return ErrCode::General_Error;
}

ErrCode CNetworkConnection::ConnectTo(const Address &address, long wait_millsec)
{
    std::cout << "Not Implemented\n";

    return ErrCode::General_Error;
}

CNetworkConnectionUDP::CNetworkConnectionUDP(INetworkObject *network_object, const int port, const int id) : CNetworkConnection(network_object, port, id)
{
    // todo some check ?
    m_UDPSocket = new QUdpSocket(this);
    m_UDPSocket->bind(QHostAddress::Any, m_iPort);
    connect(m_UDPSocket, SIGNAL(readyRead()), this, SLOT(Read()));
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
    std::cout << "Send to " << data.ToString() << " " << addr.m_IP.c_str() << " " << addr.m_iPort << std::endl;

    return ErrCode::No_Error;
}

void CNetworkConnectionUDP::Read()
{
    while(m_UDPSocket->hasPendingDatagrams()) {
        QByteArray recieved_data;
        recieved_data.resize(m_UDPSocket->pendingDatagramSize());

        QHostAddress sender_address;
        quint16 sender_port;
        m_UDPSocket->readDatagram(recieved_data.data(), recieved_data.size(), &sender_address, &sender_port);

        NetworkDataFromNS data;
        data.first = m_iID;
        data.second.SetData(recieved_data.toStdString());
        data.second.SetReceiver(Address(-1, sender_address.toString().toStdString(), sender_port));

        std::cout << "Rec from " << recieved_data.toStdString() << " " << sender_address.toString().toStdString() << " " << sender_port << std::endl;
        m_ConnectedNetworkObject->AddPendingDataFromNC(std::move(data));
    }
}



//
CNetworkConnectionTCP::CNetworkConnectionTCP(INetworkObject *network_object, const int port, const int id) : CNetworkConnection(network_object, port, id)
{
    m_bConnected = false;
}

CNetworkConnectionTCP::~CNetworkConnectionTCP()
{
}

ErrCode CNetworkConnectionTCP::Send(const CNetworkData &data)
{
    if( m_bConnected )
        m_TCPSocket->write(data.ToString().c_str());
    else
        return ErrCode::Network_Error;

    return ErrCode::No_Error;
}

ErrCode CNetworkConnectionTCP::ConnectTo(const Address &address, long wait_millsec)
{
    std::cout << "Not Implemented\n";

    return ErrCode::General_Error;
}

ErrCode CNetworkConnectionTCP::Listen(const std::string &from)
{
    std::cout << "Not Implemented\n";

    return ErrCode::General_Error;
}

void CNetworkConnectionTCP::Read()
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

void CNetworkConnectionTCP::Connected()
{
    std::cout << "Not Implemented\n";
}

void CNetworkConnectionTCP::Disconnected()
{
    std::cout << "Not Implemented\n";
}

CNetworkConnectionTCPClient::CNetworkConnectionTCPClient(INetworkObject *network_object, const int port, const int id) : CNetworkConnectionTCP(network_object, port, id)
{
    m_TCPSocket = new QTcpSocket(this);
    connect(m_TCPSocket, SIGNAL(connected()),this, SLOT(Connected()));
    connect(m_TCPSocket, SIGNAL(disconnected()),this, SLOT(Disconnected()));
    connect(m_TCPSocket, SIGNAL(readyRead()),this, SLOT(Read()));
    m_bConnected = false;
}

CNetworkConnectionTCPClient::~CNetworkConnectionTCPClient()
{
    delete m_TCPSocket;
}

void CNetworkConnectionTCPClient::Connected()
{
    m_bConnected = true;
}

void CNetworkConnectionTCPClient::Disconnected()
{
    m_bConnected = false;
}

ErrCode CNetworkConnectionTCPClient::ConnectTo(const Address &address, long wait_millsec)
{
    m_TCPSocket->connectToHost(QHostAddress(address.m_IP.c_str()), address.m_iPort);

    if( !m_TCPSocket->waitForConnected(wait_millsec) ) {
        std::cout << "Error: " << m_TCPSocket->errorString().toStdString() << std::endl;
        return ErrCode::Network_Error;
    }

    return ErrCode::No_Error;
}


CNetworkConnectionTCPServer::CNetworkConnectionTCPServer(INetworkObject *network_object, const int port, const int id) : CNetworkConnectionTCP(network_object, port, id)
{
    m_TCPServer = new QTcpServer(this);
    connect(m_TCPServer, SIGNAL(newConnection()), this, SLOT(Connected()));
}

CNetworkConnectionTCPServer::~CNetworkConnectionTCPServer()
{
    delete m_TCPServer;
}

ErrCode CNetworkConnectionTCPServer::Listen(const std::string &from)
{
    if( !m_TCPServer->listen((from.empty() ? QHostAddress::Any : QHostAddress(from.c_str())), m_iPort) )
        return ErrCode::Network_Error;

    return ErrCode::No_Error;
}

void CNetworkConnectionTCPServer::Connected()
{
    if( !m_bConnected ) {
        m_TCPSocket = m_TCPServer->nextPendingConnection();
        connect(m_TCPSocket, SIGNAL(readyRead()), this, SLOT(Read()));
        connect(m_TCPSocket, SIGNAL(disconnected()),this, SLOT(Disconnected()));
        m_bConnected = true;
    } else {
        //todo
        std::cout << "Someone try to connect, but we are connected now\n";
    }
}

void CNetworkConnectionTCPServer::Disconnected()
{
    m_bConnected = false;
}
