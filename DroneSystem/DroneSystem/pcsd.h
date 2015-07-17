#include <vector>
#include <map>
#include <QUdpSocket>

typedef int address;
typedef int scene;
typedef int task;


class DroneSystem
{
public:

    DroneSystem(address init_space, address syn_space, address coord_space, int ID);
	int InitSys();
    void PrintIDs();
private:
	//address data
	address m_InitSpace;
	address m_SynSpace;
	address m_CoordSpace;

    QUdpSocket *udpSocket;
    int port;
	
	//drone system data
	int m_ID;
	int m_CoordID;
	address m_CoordAddress;
	int m_NextID;
	address m_NextAddress;
	std::vector<int> m_DSIDs;
	std::map<int, address> m_DSIDToAddress;
	
	//work data
	scene m_CurrScene;
};
