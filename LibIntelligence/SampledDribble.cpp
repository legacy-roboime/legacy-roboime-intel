#include "SampledDribble.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "Sampler.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

SampledDribble::SampledDribble(QObject* parent, Robot* slave, Object* lookPoint, qreal minPower, qreal maxPower, qreal speed)
	: DriveToBall(parent, slave, lookPoint, speed),
	minPower_(minPower),
	maxPower_(maxPower)
{
	//this->setObjectName("SampledDribble");
}

SampledDribble::~SampledDribble(void)
{
}

void SampledDribble::step()
{
	Robot* robot = this->robot();

	//lookPoint = Object(*refLookPoint_);
	DriveToBall::step();
	
	if(!this->busy()){
		robot->dribble(Sampler::sampledPowerDribble(minPower_, maxPower_));
	}
	else {
		robot->dribble(0.);
	}
}

bool SampledDribble::busy()
{
	return DriveToBall::busy();
}