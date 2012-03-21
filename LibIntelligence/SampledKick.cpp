#include "SampledKick.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "Sampler.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

SampledKick::SampledKick(QObject* parent, Robot* slave, Object* lookPoint, qreal minPower, qreal maxPower, qreal speed)
	: DriveToBall(parent, slave, lookPoint, speed),
	minPower_(minPower),
	maxPower_(maxPower)
{
	//this->setObjectName("SampledKick");
}

SampledKick::~SampledKick(void)
{
}

void SampledKick::step()
{
	Robot* robot = this->robot();
	Stage* stage = this->stage();
	Goal* enemyGoal = stage->getGoalFromOtherColor(robot->color());
	lookPoint = Object(*refLookPoint_);
	static qreal y = DriveToBall::lookPoint.y();
	static qreal power = Sampler::sampledPowerKick(minPower_, maxPower_);
	
	if(!this->busy()){
		power = Sampler::sampledPowerKick(minPower_, maxPower_);
		y = DriveToBall::lookPoint.y();
		qreal width = enemyGoal->width();
		width /= 2;
		width *= Sampler::randFloat();
		qreal signal = Sampler::randFloat();
		if(signal < 0.5)
			signal = -1.;
		else
			signal = 1.;
		y += width * signal;
		//cout << "SetY: " << y << endl;

		robot->kick(power);
	}
	else {
		robot->kick(0.);
	}

	DriveToBall::lookPoint.setY(y);
	DriveToBall::step();
}

bool SampledKick::busy()
{
	return DriveToBall::busy();
}