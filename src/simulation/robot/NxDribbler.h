#pragma once

#include "NxArray.h"
#include "NxShape.h"

class NxDribbler
{
private:
public:
	float speedToExecute;
	NxArray<NxShape*> dribblerShapes;
	void controlDribbler( float dribblerSpeed );
	NxVec3 getLocalPosition();
	NxDribbler(void);
	~NxDribbler(void);
};
