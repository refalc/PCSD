#ifndef FREQUENCY
#define FREQUENCY

#include <string>
#include <set>
#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include "../../Utils/Utils/utils.h"

class SynFrequency : public QObject
{
    Q_OBJECT
public:
    SynFrequency(int port, QObject *parent = 0);
    void Start();
    void Close();
    void Send(Address dst);

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
    QTcpServer m_Server;
    QTcpSocket *m_Client;
    QUdpSocket *m_Socket;
    int m_Port;
    std::map<int, Address> m_Space;
};


#endif // FREQUENCY

