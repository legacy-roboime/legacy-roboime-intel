#include "SampledKick.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "Sampler.h"
#include "config.h"
#include "mathutils.h"

#ifdef SIMU
#define KICKPOWERK 8000
#else
#define KICKPOWERK 1000
#endif

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

SampledKick::SampledKick(QObject* parent, Robot* slave, Object* lookPoint, bool deterministic, qreal minPower, qreal maxPower, qreal speed, bool pass)
	: DriveToBall(parent, slave, lookPoint, speed, deterministic, 15, /*5 * */50., /*20 * */5 * M_PI/180.),
	minPower_(minPower),
	maxPower_(maxPower),
	powerK(KICKPOWERK),
	pass_(pass)
{
	//this->setObjectName("SampledKick");
	//threshold = CART;
}

void SampledKick::setPowerK(qreal k)
{
	powerK = k;
}

qreal SampledKick::calculatePassPower(qreal s)
{
	qreal mi = .4;
	qreal g = 9810;
	qreal vFin = 0;
	qreal vInit = 0;
	qreal a = mi*g;
	qreal power;
	vInit = sqrt(vFin*vFin + 2*a*s); 
	power = vInit/powerK;
	return power*power;
}

void SampledKick::step()
{
	Robot* robot = this->robot();
	Ball* ball = this->stage()->ball();

	//cout << sqrt(ball->speed().x()*ball->speed().x() + ball->speed().y()*ball->speed().y()) << endl;
	
	if(!busy()){
		qreal power;
		if(pass_) {
			qreal distReal = Vector(*ball - *getLookPoint()).length();
			power = calculatePassPower(distReal);
		} else {
			if(deterministic_)
				power = maxPower_;
			else
				power = Sampler::sampledPowerKick(minPower_, maxPower_);
		}

		robot->kick(power);
	}

	//robot->dribble(0.5); //pegar bola

	DriveToBall::step();
}

bool SampledKick::busy()
{
	return DriveToBall::busy();//false;//
}
