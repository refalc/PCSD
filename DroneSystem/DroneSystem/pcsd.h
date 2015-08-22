#include <vector>
#include <map>
#include <QUdpSocket>
#include <QTcpSocket>
#include "../Utils/Utils/utils.h"
#include <deque>

class DroneSystem : QObject
{
    Q_OBJECT
public:

    DroneSystem(int ID, int SUPort, int TCPPort, Address &Syn, QObject *parent = 0);
    void DoSync();
    void Work();

private:
    bool IsEqualState(std::map<int, Address> &state);
    void SendIDToSync();
    void SendNext(QByteArray &data);
    void DecodeTask(std::string inputData);
    void DoTaskIteration();
    void SendTcp(std::string command);
    void DoTask();
    void DecodeCommand(std::string command);

signals:

public slots:
    void ReadFromSynUDP();
    void ReadFromLastDrone();

    void ConnectedTcp();
    void DisconnectedTcp();
    void ReadTcp();

private:
	//address data
    Address m_SynF;
    Address m_NextDrone;
    Address m_LastDrone;

    QUdpSocket *m_SynUdpSocket;
    int m_SynUdpPort;
    QUdpSocket *m_CrossDroneUdpSocket;
    int m_CrossDroneUdpPort;
    QTcpSocket *m_TcpSocket;
    int m_TcpPort;
	
	//drone system data
	int m_ID;
    double m_Crd[3];
    bool m_isCoordinator;
    bool m_Alone;
    bool m_OkTask;
	
    //coordinator data
    std::vector<Iteration> m_AllCTasks;

	//work data
    std::deque<std::string> m_Tasks;
    QString m_CurrScene;
    std::vector<int> m_DroneIDs;
    std::map<int, Address> m_DronesSpace;
};
