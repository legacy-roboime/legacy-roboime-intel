#pragma once
#include <NxScene.h>
//#include <stdio.h>
//#include <string.h>
//#include <sstream>
//#include <iostream>
//#include "NxPhysics.h"
#include "NxAllRobots.h"
#include "NxField.h"
#include "NxBall.h"
//#include <NxArray.h>

//using namespace std;

class Simulation;

class NxScene1
{
private:
	Simulation* simulation;
public:
	int indexScene;
	NxScene* scene;
	NxAllRobots* allRobots;
	NxField* field;
	NxBall* ball;
public:
	friend class NxScene;

	NxScene1(Simulation* simulation);
	~NxScene1(void);
	Simulation* getSimulation();
	bool operator<(const NxScene1 &scene);
	bool operator>(const NxScene1 &scene);
	bool operator<=(const NxScene1 &scene);
	bool operator>=(const NxScene1 &scene);
	bool operator==(const NxScene1 &scene);
};
