#pragma once
#include "NxBall.h"
#include <NxArray.h>

class NxAllBalls
{
public:
	NxArray<NxBall> balls;
public:
	NxAllBalls(void);
	~NxAllBalls(void);
	NxBall getBallByScene(int indexScene);
	//bool isBall(NxBall ball);
};
