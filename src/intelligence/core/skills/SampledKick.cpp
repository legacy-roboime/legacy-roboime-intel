#include "SampledKick.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "Sampler.h"
#include "config.h"

//#define CART	90.//100.//110.//82.6

#ifdef SIMU
#define KICKPOWERK 8000
#else
#define KICKPOWERK 1000
#endif

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

SampledKick::SampledKick(QObject* parent, Robot* slave, Object* lookPoint, bool deterministic, qreal minPower, qreal maxPower, qreal speed, bool pass)
	: DriveToBall(parent, slave, lookPoint, speed, deterministic, 15, 50., 5 * M_PI/180.),
	minPower_(minPower),
	maxPower_(maxPower),
	pass_(pass),
	powerK(KICKPOWERK)
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

	//cout << sqrt(ball->speedX()*ball->speedX() + ball->speedY()*ball->speedY()) << endl;
	
	if(!busy()){
		qreal power;
		if(!deterministic_){
			if(pass_){
				const Object* lkPoint = getLookPoint();
				Object* distVec = &(ball->distance(lkPoint));
				qreal distReal = distVec->module();
				power = Sampler::sampledPowerKick(minPower_, calculatePassPower(distReal));
			}
			else
				power = Sampler::sampledPowerKick(minPower_, maxPower_);
		}
		else{
			if(pass_){
				const Object* lkPoint = getLookPoint();
				Object* distVec = &(ball->distance(lkPoint));
				qreal distReal = distVec->module();
				power = calculatePassPower(distReal);
			}
			else
				power = maxPower_;
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