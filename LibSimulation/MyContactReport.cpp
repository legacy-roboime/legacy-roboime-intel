#include "MyContactReport.h"
#include "Simulation.h"
//#include "NxAllRobots.h"

MyContactReport::MyContactReport(Simulation* simulation)
{
	this->simulation = simulation;
}

MyContactReport::~MyContactReport(void)
{
	delete simulation;
}

void  MyContactReport::onContactNotify(NxContactPair& pair, NxU32 events)
{
	NxI32 robotIndex = -1;
	//TODO: Executar apenas para cena que contem o pair
	for(int i=0; i<simulation->gScenes.size(); i++){
		simulation->gScenes[i]->allRobots->handlePair(pair, events);	
	}
}