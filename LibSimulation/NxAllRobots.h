#ifndef NX_ALL_ROBOTS
#define NX_ALL_ROBOTS

#include <set>
#include "NxRobot.h"

class NxAllRobots {
private:
	//TODO: JAH DEIXAR SEPARADOS OS ROBOS POR CENA PARA NAO TER Q FICAR REALiZANDO BUSCA
	std::set<NxRobot*> _allRobots;
	NxArray<NxRobot*> _allRobotsSequential;
	std::set<NxRobot*> _allChildRobots;
	
	NxI32 _activeRobot;
	NxRobot* _activeRobotP;
	void setActiveRobotP();
public:
	void AddRobot(NxRobot* v);
	void AddChildRobot(NxRobot* v);
	//void RemoveRobot(NxRobot* v);

	void updateAllRobots(NxReal lastTimeStep);
	void drawRobots(bool debug = false);

	bool isRobot(NxRobot* v);
	void handlePair(NxContactPair& pair, NxU32 events);

	NxU32 getNumberOfRobots() { return (NxU32)_allRobots.size(); }
	void setActiveRobot(NxI32 v) { _activeRobot = v; setActiveRobotP(); }
	NxRobot* getActiveRobot() { return _activeRobotP; }
	NxI32 getActiveRobotNumber() { return _activeRobot; }
	void selectNext();
	
	NxArray<NxRobot*> getRobotsByTeam(int idTeam);
	NxRobot* getRobotByIdByTeam(int indexRobot, int idTeam);
	NxRobot* getRobotById(int indexRobot);
	NxArray<NxRobot*> getRobots();
	int getBiggestIndexRobot();
	NxAllRobots();
	~NxAllRobots();
};

#endif
