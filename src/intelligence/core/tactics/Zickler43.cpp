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
using namespace Zickler43T;

Zickler43::Zickler43(QObject* p, Robot* r, qreal speed, bool deterministic)
	: Tactic(p,r, deterministic),
	driveToBall_(new DriveToBall(this, r, r->enemyGoal(), speed, true)),
	sampledDribble_(new SampledDribble(this, r, r->enemyGoal(), deterministic, 0., 1., speed/.6)),
	sampledGoalKick_(new SampledKick(this, r, r->enemyGoal(), deterministic, 0.9, 1., speed/.6, false)),
	sampledMiniKick_(new SampledKick(this, r, r->enemyGoal(), deterministic, 0., 0.3, speed/.6, false)),
	wait_(new Wait(p, r)),
	speed(speed)
{
	this->pushState(driveToBall_);//this is important to destructor
	this->pushState(sampledGoalKick_);
	this->pushState(sampledDribble_);
	this->pushState(sampledMiniKick_);
	this->pushState(wait_);

	driveToBall_->setObjectName("DriveToBall");
	sampledDribble_->setObjectName("SampledDribble");
	sampledGoalKick_->setObjectName("SampledGoalKick");
	sampledMiniKick_->setObjectName("SampledMiniKick");
	wait_->setObjectName("Wait");

	this->pushTransition(driveToBall_, new DriveToDribbleT(this, driveToBall_, sampledDribble_, 1.));
	this->pushTransition(sampledDribble_, new DribbleToDriveT(this, sampledDribble_, driveToBall_, 1.));
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

//void Zickler43::step()
//{
//	Skill* current = (Skill*)this->getCurrentState();
//	this->execute();
//	current->step();
//	//cout << current->objectName().toStdString() << endl;
//}

bool DriveToDribbleT::condition()
{
	Zickler43* z = (Zickler43*) this->parent();
	Robot* r = z->robot();
	Ball*b = z->stage()->ball();
	return !source_->busy(); //r->distance(b).module() < 400;//
}

DriveToDribbleT::DriveToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToDriveT::condition()
{
	SampledDribble* dribble = (SampledDribble*)source_;
	DriveToBall* drive = (DriveToBall*)target_;
	const Object* backup = drive->getRefLookPoint();
	drive->setRefLookPoint(dribble->getRefLookPoint());
	bool busy = drive->busy();
	drive->setRefLookPoint(backup);
	return busy;
}

DribbleToDriveT::DribbleToDriveT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

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