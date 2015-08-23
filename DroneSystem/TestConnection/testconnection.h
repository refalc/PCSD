#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <iostream>
#include "E:\soft\PCSD\DroneSystem\Utils\Utils\utils.h"

class TestConnection : public QObject
{
    Q_OBJECT
public:
    explicit TestConnection(int port, QObject *parent = 0);
    void Send(std::string ID);
    void SendTcp(std::string data);
    void SpamMove();

    //tcp test
    void DoConnect();
signals:

public slots:
    void Read();

    //tcp test
    void Connected();
    void Disconnected();
    void ReadTcp();
    void Write(qint64 bytes);

private:
    QUdpSocket *m_Socket;
    QTcpSocket *m_TcpSocket;
    std::map<int, Address> m_Space;
    int m_Port;
    bool m_Connect;
};

#endif // TESTCONNECTION_H

