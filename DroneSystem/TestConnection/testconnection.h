#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H

#include <QObject>
#include <QUdpSocket>
#include <iostream>
#include "E:\soft\PCSD\DroneSystem\Utils\Utils\utils.h"

class TestConnection : public QObject
{
    Q_OBJECT
public:
    explicit TestConnection(int port, QObject *parent = 0);
    void Send(std::string ID);
    ~TestConnection();
signals:

public slots:
    void Read();

private:
    QUdpSocket *m_Socket;
    std::map<int, Address> m_Space;
    int m_Port;
};

#endif // TESTCONNECTION_H

