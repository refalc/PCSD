#include <vector>
#include <map>
#include <QUdpSocket>
#include <QTcpSocket>
#include "../Utils/Utils/utils.h"
#include <deque>

struct NetAddress
{
    QHostAddress IP;
    int PORT;
};

class DroneSystem : QObject
{
    Q_OBJECT
public:

    DroneSystem(int ID, int SUPort, NetAddress Syn, QObject *parent = 0);
    void DoSync();
    void Work();

private:
    bool IsEqualState(std::map<int, Address> &state);
    void SendIDToSync();

signals:

public slots:
    void ReadFromSynUDP();

private:
	//address data
    NetAddress m_SynF;
    NetAddress m_NextDrone;
    NetAddress m_LastDrone;

    QUdpSocket *m_SynUdpSocket;
    int m_SynUdpPort;
    QUdpSocket *m_UdpSocket;
    int m_UdpPort;
    QTcpSocket *m_TcpSocket;
    int m_TspPort;
	
	//drone system data
	int m_ID;
    double m_Coord[3];
    bool m_isCoordinator;
	
	//work data
    std::deque<int> m_Tasks;
    QString m_CurrScene;
    std::vector<int> m_DroneIDs;
    std::map<int, Address> m_DronesSpace;
};
