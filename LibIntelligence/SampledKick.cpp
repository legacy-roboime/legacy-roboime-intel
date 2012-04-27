#include "SampledKick.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "Sampler.h"

#define CART	82.6

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

SampledKick::SampledKick(QObject* parent, Robot* slave, Object* lookPoint, bool deterministic, qreal minPower, qreal maxPower, qreal speed, bool pass)
	: DriveToBall(parent, slave, lookPoint, speed, deterministic, 15),
	minPower_(minPower),
	maxPower_(maxPower),
	pass_(pass)
{
	//this->setObjectName("SampledKick");
	threshold = CART;
}

SampledKick::~SampledKick(void)
{
}

void SampledKick::step()
{
	Robot* robot = this->robot();

	qreal power;
	if(!deterministic_){
		if(pass_){
			const Object* lkPoint = getLookPoint();
			Object* distVec = &(robot->distance(lkPoint));
			qreal distReal = distVec->module();
			distReal /= 3000.;
			power = Sampler::sampledPowerKick(minPower_, distReal);
		}
		else
			power = Sampler::sampledPowerKick(minPower_, maxPower_);
	}
	else{
		if(pass_){
			const Object* lkPoint = getLookPoint();
			Object* distVec = &(robot->distance(lkPoint));
			qreal distReal = distVec->module();
			distReal /= 3000.;
			power = distReal;
		}
		else
			power = maxPower_;
	}

	robot->kick(power);
	robot->dribble(0.5); //pegar bola

	DriveToBall::step();
}

bool SampledKick::busy()
{
	return false;//DriveToBall::busy();
}