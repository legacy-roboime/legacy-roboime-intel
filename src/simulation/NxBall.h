#pragma once

#include "NxActor.h"

class Simulation;

class NxBall
{
public:
	int indexScene;
	Simulation* simulation;
	NxActor* ball; 
	NxMat34 initialPose;
public:
	NxBall(Simulation* simulation);
	~NxBall(void);

	void cloneBall(int indexNewScene);
	void resetToInitialPose();
	void putToSleep();
};
