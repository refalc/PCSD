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
    void Send(const Address &dst);
    void Send(const std::string &data);

    //tcp test
    void DoConnect();

signals:

public slots:
    void Read();

    //tcp test
    void Connected();
    void Disconnected();
    void ReadTcp(int pos);
    void Write(qint64 bytes);

protected:
    QTcpServer m_Server;
    QTcpSocket *m_Client;

    std::vector<QTcpSocket*> m_Clients;
    QUdpSocket *m_Socket;
    int m_Port;
    std::map<int, Address> m_Space;
};


#endif // FREQUENCY

