#include "SampledDribble.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "Sampler.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

SampledDribble::SampledDribble(QObject* parent, Robot* slave, Object* lookPoint, bool deterministic, qreal minPower, qreal maxPower, qreal speed)
	: DriveToBall(parent, slave, lookPoint, speed, deterministic, 60.),
	minPower_(minPower),
	maxPower_(maxPower)
{}

SampledDribble::~SampledDribble(void)
{}

void SampledDribble::step()
{
	Robot* robot = this->robot();

	if(!deterministic_){
		robot->dribble(Sampler::sampledPowerDribble(minPower_, maxPower_));
	}
	else{
		robot->dribble(maxPower_);
	}

	qreal backup = threshold;
	threshold = .001; //para que o robô conduza numa direção é feita uma diminuição do threshold
	DriveToBall::step();
	threshold = backup;
}

bool SampledDribble::busy()
{
	return DriveToBall::busy();//false;//
}