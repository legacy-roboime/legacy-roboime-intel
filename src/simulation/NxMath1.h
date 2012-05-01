#pragma once
#include "NxMath.h"
#include "Nxf.h"
#include "NxMat33.h"

class NxMath1 :	public NxMath
{
public:
	//Math
	static NxReal getAngle2DFromMat33( NxMat33 matrixOrientation );
	static NxF32 calcDistanceVec2D( NxF32 x1, NxF32 y1, NxF32 x2, NxF32 y2 );
	static NxReal getBiggestAbsoluteValue(NxReal* values, int size);
	NxMath1(void);
	~NxMath1(void);
};
