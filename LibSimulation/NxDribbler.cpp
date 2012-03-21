#include "NxDribbler.h"

NxDribbler::NxDribbler(void)
{
	speedToExecute = 0;
}

NxDribbler::~NxDribbler(void)
{
}

void NxDribbler::controlDribbler( float dribblerSpeed )
{
	this->speedToExecute = dribblerSpeed;
}

NxVec3 NxDribbler::getLocalPosition(){
	NxVec3 pos = NxVec3(0,0,0);
	for(int i=0; i<dribblerShapes.size(); i++){
		pos += dribblerShapes[i]->getLocalPosition() / ((float)(dribblerShapes.size()));
	}
	return pos;
}
