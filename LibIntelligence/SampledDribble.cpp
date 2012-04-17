#include "SampledDribble.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "Sampler.h"

#define CART	82.6

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

SampledDribble::SampledDribble(QObject* parent, Robot* slave, Object* lookPoint, bool deterministic, qreal minPower, qreal maxPower, qreal speed)
	: DriveToBall(parent, slave, lookPoint, speed, deterministic),
	minPower_(minPower),
	maxPower_(maxPower)
{
	//this->setObjectName("SampledDribble");
	threshold = CART;

}

SampledDribble::~SampledDribble(void)
{
}

void SampledDribble::step()
{
	Robot* robot = this->robot();

	if(!deterministic_){
		robot->dribble(Sampler::sampledPowerDribble(minPower_, maxPower_));
	}
	else{
		robot->dribble(maxPower_);
	}

	DriveToBall::step();
}

bool SampledDribble::busy()
{
	return false;//DriveToBall::busy();
}