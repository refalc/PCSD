#ifndef FREQUENCY
#define FREQUENCY

#include <string>
#include <set>
#include <QObject>
#include <QUdpSocket>
#include "../../Utils/Utils/utils.h"

class SysFrequency : public QObject
{
    Q_OBJECT
public:
    SysFrequency(int port, QObject *parent = 0);
    void Start();
    void Close();
    void Send(Address dst);

signals:

public slots:
    void Read();

private:
    QUdpSocket *m_Socket;
    int m_Port;
    std::map<int, Address> m_Space;
};


#endif // FREQUENCY

