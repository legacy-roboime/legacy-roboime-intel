
#include "NxScene1.h"
#include "Simulation.h"

NxScene1::NxScene1(Simulation* simulation)
{
	this->simulation = simulation;
	allRobots = new NxAllRobots();
	field = new NxField(simulation);
	ball = new NxBall(simulation);
}

NxScene1::~NxScene1(void)
{
	delete allRobots;
	delete field;
	delete ball;
	if (scene)
	{
		simulation->gPhysicsSDK->releaseScene(*(scene));
		scene = NULL;
	}
}

Simulation* NxScene1::getSimulation()
{
	return simulation;
}

bool NxScene1::operator<(const NxScene1 &scene){
	return (this->indexScene < scene.indexScene);
}

bool NxScene1::operator>(const NxScene1 &scene){
	return (this->indexScene > scene.indexScene);
}

bool NxScene1::operator<=(const NxScene1 &scene){
	return (this->indexScene <= scene.indexScene);
}

bool NxScene1::operator>=(const NxScene1 &scene){
	return (this->indexScene >= scene.indexScene);
}

bool NxScene1::operator==(const NxScene1 &scene){
	return (this->indexScene == scene.indexScene);
}