#include "Zickler43.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include "Sampler.h"
#include <QLineF>
#include <iostream>

#define M_PI	3.1415926535897932

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Zickler43::Zickler43(QObject* p, Robot* r, qreal speed, bool deterministic)
	: Tactic(p,r, deterministic),
	driveToBall_(new DriveToBall(this, r, NULL, 3000)),
	sampledDribble_(new SampledDribble(this, r, NULL, 0., 1., 1000.)),
	sampledGoalKick_(new SampledKick(this, r, NULL, 0.9, 1., 1000.)),
	sampledMiniKick_(new SampledKick(this, r, NULL, 0., 0.3, 1000.)),
	wait_(new Wait(p, r)),
	speed(speed)
{
	this->pushState(driveToBall_);//this is important to destructor
	this->pushState(sampledGoalKick_);
	this->pushState(sampledDribble_);
	this->pushState(sampledMiniKick_);
	this->pushState(wait_);

	Goal* enemyGoal = this->robot()->enemyGoal();
	driveToBall_->setRefLookPoint(enemyGoal);
	sampledDribble_->setRefLookPoint(enemyGoal);
	sampledMiniKick_->setRefLookPoint(enemyGoal);
	sampledGoalKick_->setRefLookPoint(enemyGoal);

	driveToBall_->setObjectName("DriveToBall");
	sampledDribble_->setObjectName("SampledDribble");
	sampledGoalKick_->setObjectName("SampledGoalKick");
	sampledMiniKick_->setObjectName("SampledMiniKick");
	wait_->setObjectName("Wait");

	this->pushTransition(driveToBall_, new DriveToDribbleT(this, driveToBall_, sampledDribble_, 1.));
	this->pushTransition(sampledDribble_, new DribbleToGoalKickT(this, sampledDribble_, sampledGoalKick_, .1));
	this->pushTransition(sampledDribble_, new DribbleToMiniKickT(this, sampledDribble_, sampledMiniKick_, .2));
	this->pushTransition(sampledDribble_, new DribbleToDribbleT(this, sampledDribble_, sampledDribble_, .8));
	this->pushTransition(sampledMiniKick_, new DefaultTrueT(this, sampledMiniKick_, driveToBall_));
	this->pushTransition(sampledGoalKick_, new DefaultTrueT(this, sampledGoalKick_, wait_));
	this->pushTransition(wait_, new DefaultTrueT(this, wait_, driveToBall_)); //loop maquina

	this->setInitialState(driveToBall_);

	this->reset();
}

//Zickler43::Zickler43(QObject* p, Robot* r, const Zickler43& zickler) : Tactic(p, r, zickler)
//{
//	this->speed = zickler.speed;
//	*driveToBall_ = *zickler.driveToBall_;
//	*sampledDribble_ = *zickler.sampledDribble_;
//	*sampledGoalKick_ = *zickler.sampledGoalKick_;
//	*sampledMiniKick_ = *zickler.sampledMiniKick_;
//	*wait_ = *zickler.wait_;
//
//	this->pushState(driveToBall_);//this is important for destructor
//	this->pushState(sampledGoalKick_);
//	this->pushState(sampledDribble_);
//	this->pushState(sampledMiniKick_);
//	this->pushState(wait_);
//
//	Goal* enemyGoal = this->robot()->enemyGoal();
//	driveToBall_->setRefLookPoint(enemyGoal);
//	sampledDribble_->setRefLookPoint(enemyGoal);
//	sampledMiniKick_->setRefLookPoint(enemyGoal);
//	sampledGoalKick_->setRefLookPoint(enemyGoal);
//
//	this->createTransition(this, "driveToDribble", driveToBall_, sampledDribble_, 1.);
//	this->createTransition(this, "dribbleToGoalKick", sampledDribble_, sampledGoalKick_, .1);
//	this->createTransition(this, "dribbleToMiniKick", sampledDribble_, sampledMiniKick_, .2);
//	this->createTransition(this, "dribbleToDribble", sampledDribble_, sampledDribble_, .8);
//	this->createTransition(this, "defaultTrue", sampledMiniKick_, driveToBall_);
//	this->createTransition(this, "defaultTrue", sampledGoalKick_, wait_);
//	this->createTransition(this, "defaultTrue", wait_, driveToBall_); //loop maquina
//}

Zickler43::~Zickler43()
{
	delete driveToBall_;
	delete sampledDribble_;
	delete sampledGoalKick_;
	delete sampledMiniKick_;
	delete wait_;
}

void Zickler43::step()
{
	Skill* current = (Skill*)this->getCurrentState();
	this->execute();
	current->step();
	//cout << current->objectName().toStdString() << endl;
}

bool DriveToDribbleT::condition()
{
	return !source_->busy();
}

DriveToDribbleT::DriveToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToDribbleT::condition()
{
	return !source_->busy();
}

DribbleToDribbleT::DribbleToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToGoalKickT::condition()
{
	return !source_->busy();
}

DribbleToGoalKickT::DribbleToGoalKickT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToMiniKickT::condition()
{
	return !source_->busy();
}

DribbleToMiniKickT::DribbleToMiniKickT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DefaultTrueT::condition()
{
	return true;
}

DefaultTrueT::DefaultTrueT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}