#include "NxAllRobots.h"

void NxAllRobots::AddRobot(NxRobot* v) {
	if (!isRobot(v)) {
		_allRobots.insert(v);
		_allRobotsSequential.pushBack(v);
	}
}

void NxAllRobots::AddChildRobot(NxRobot* v) {
	if (!isRobot(v)) {
		_allChildRobots.insert(v);
	}
}
/*
void NxAllRobots::RemoveRobot(NxRobot* v) {
	if (isRobot(v)) {
		_allRobots.erase(v);
		NxU32 i = 0;
		for (; i < _allRobotsSequential.size(); i++)
			if (_allRobotsSequential[i] == v)
				break;
		_allRobotsSequential.replaceWithLast(i);
	} else {
		printf("Robot not in List\n");
	}
}
*/
void NxAllRobots::updateAllRobots(NxReal lastTimeStep) {
	std::set<NxRobot*>::iterator it = _allRobots.begin();
	for (;it != _allRobots.end(); ++it) {
		(*it)->updateVehicle(lastTimeStep);
	}
	for (it = _allChildRobots.begin(); it != _allChildRobots.end(); ++it) {
		(*it)->updateVehicle(lastTimeStep);
	}
	//printf("\n");
}

bool NxAllRobots::isRobot(NxRobot* v) {
	std::set<NxRobot*>::iterator it = _allRobots.find(v);
	if (it != _allRobots.end())
		return true;

	it = _allChildRobots.find(v);
	return it != _allChildRobots.end();
}

void NxAllRobots::setActiveRobotP() {
	if (_activeRobot < 0) {
		_activeRobotP = NULL;
		return;
	}
	_activeRobotP = _allRobotsSequential[_activeRobot];
}

void NxAllRobots::drawRobots(bool debug) {
	for (NxU32 i = 0; i < _allRobotsSequential.size(); i++) {
		_allRobotsSequential[i]->draw(debug);
	}
	std::set<NxRobot*>::iterator it = _allRobots.begin();
	for (it = _allChildRobots.begin(); it != _allChildRobots.end(); ++it) {
		(*it)->draw(debug);
	}
}

void NxAllRobots::selectNext() {
	_activeRobot++;
	if (_activeRobot >= (NxI32)_allRobots.size())
		_activeRobot = -1;
	setActiveRobotP();
}

void NxAllRobots::handlePair(NxContactPair& pair, NxU32 events) {
	if (isRobot((NxRobot*)pair.actors[0]->userData)) {
		NxRobot* v = (NxRobot*)pair.actors[0]->userData;
		v->handleContactPair(pair, 0);
	}
	if (isRobot((NxRobot*)pair.actors[1]->userData)) {
		NxRobot* v = (NxRobot*)pair.actors[1]->userData;
		v->handleContactPair(pair, 1);
	}
}

NxRobot* NxAllRobots::getRobotByIdByTeam(int indexRobot, int idTeam){
	for (NxU32 i = 0; i < _allRobotsSequential.size(); i++) {
		if(_allRobotsSequential[i]!=NULL){
			if(_allRobotsSequential[i]->getId()==indexRobot && _allRobotsSequential[i]->getIdTeam()==idTeam)
				return _allRobotsSequential[i];
		}
	}
	return NULL;
}

NxArray<NxRobot*> NxAllRobots::getRobotsByTeam(int idTeam){
	NxArray<NxRobot*> robots;
	for (NxU32 i = 0; i < _allRobotsSequential.size(); i++) {
		if(_allRobotsSequential[i]!=NULL){
			if(_allRobotsSequential[i]->getIdTeam()==idTeam)
				robots.push_back(_allRobotsSequential[i]);
		}
	}
	return robots;
}

NxArray<NxRobot*> NxAllRobots::getRobots(){
	return _allRobotsSequential;
}

int NxAllRobots::getBiggestIndexRobot()
{
	int biggest = -1;
	for (NxU32 i = 0; i < _allRobotsSequential.size(); i++) {
		if(_allRobotsSequential[i]->getId()>biggest)
			biggest=_allRobotsSequential[i]->getId();
	}
	return biggest;
}

NxAllRobots::NxAllRobots(){
	_activeRobot = -1;
}

NxAllRobots::~NxAllRobots()
{
	//if(_activeRobotP)
	//	delete _activeRobotP;
	for (NxU32 i = 0; i < _allRobotsSequential.size(); i++) {
		delete _allRobotsSequential[i];
	}
	std::set<NxRobot*>::iterator it = _allRobots.begin();
	/*for (;it != _allRobots.end(); ++it) {
		if((*it))
			delete (*it);
	}	*/
	for (it = _allChildRobots.begin(); it != _allChildRobots.end(); ++it) {
		delete (*it);
	}
}