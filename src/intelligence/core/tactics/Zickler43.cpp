#include "Zickler43.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include "Sampler.h"
#include <iostream>
#include "mathutils.h"

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;
using namespace Zickler43T;

#define MINDIST 500

Zickler43::Zickler43(QObject* p, Robot* r, qreal speed, bool deterministic)
	: Tactic(p,r, deterministic),
	driveToBall_(new DriveToBall(this, r, lookPoint, speed, true)),
    sampledDribble_(new SampledDribble(this, r, lookPoint, deterministic, 0., 1., speed)),
    sampledGoalKick_(new SampledKick(this, r, lookPoint, deterministic, 0.9, 1., speed, false)),
    sampledMiniKick_(new SampledKick(this, r, lookPoint, deterministic, 0., 0.3, speed, false)),
    wait_(new Wait(p, r)),
	lookPoint(new Object())
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
    //this->pushTransition(sampledDribble_, new DribbleToMiniKickT(this, sampledDribble_, sampledMiniKick_, .2));
    //this->pushTransition(sampledDribble_, new DribbleToDribbleT(this, sampledDribble_, sampledDribble_, .8));
    //this->pushTransition(sampledMiniKick_, new DefaultTrueT(this, sampledMiniKick_, driveToBall_));
	this->pushTransition(sampledGoalKick_, new GoalKickToDriveT(this, sampledGoalKick_, driveToBall_));
	//this->pushTransition(wait_, new DefaultTrueT(this, wait_, driveToBall_)); //loop maquina

	this->setInitialState(driveToBall_);

	this->reset();

	driveToBall_->setRefLookPoint(lookPoint);
	sampledDribble_->setRefLookPoint(lookPoint);
	sampledGoalKick_->setRefLookPoint(lookPoint);
	sampledMiniKick_->setRefLookPoint(lookPoint);

    maxHoleSize = -1;
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
	delete lookPoint;
}

void Zickler43::step()
{
	Skill* current = (Skill*)this->getCurrentState();
	updateLookPoint();
	current->step();
	this->execute();
    //cout << "SKILL: " << current->objectName().toStdString() << endl;
}

void Zickler43::updateLookPoint()
{
	Point p = pointToKick();
	lookPoint->setX(p.x());
	lookPoint->setY(p.y());
}

Point Zickler43::pointToKick() //chuta no meio do maior buraco
{
	Goal* enemyGoal = this->robot()->enemyGoal();
	qreal hwidth = enemyGoal->width()/2;

	qreal initial = enemyGoal->y()-hwidth;

	Point p(enemyGoal->x(), initial);
	qreal holeSize = 0;

	Point centerMax;
	centerMax.setX(enemyGoal->x());
    maxHoleSize = -1;

	qreal delta = hwidth/5;

	for(qreal t = initial; t < enemyGoal->y()+hwidth; t+=delta){ //10 pontos
		if( isKickScored( Point(enemyGoal->x(),t) ) ){
			holeSize+=delta;
		}
		else{
			if(holeSize>maxHoleSize){
				maxHoleSize = holeSize;
				centerMax.setY(p.y() + holeSize/2);
			}
			holeSize = 0;
			p.setY(t+delta);
		}
	}
	if(holeSize>=maxHoleSize){
        maxHoleSize = holeSize;
		centerMax.setY(p.y() + holeSize/2);
	}
	return centerMax;
}

qreal Zickler43::holeSize()
{
    return maxHoleSize;
}

bool Zickler43::isKickScored( Point kickPoint )
{
	Ball* ball = this->stage()->ball();
	Line ball_kickpoint = Line(*ball, kickPoint);
	Team* team = this->robot()->team();
	for(int i=0; i<team->size(); i++){
		Robot* obst = team->at(i);
			Point obstP(obst->x(), obst->y());
			Line ball_obst = Line(*ball, obstP);
			qreal dist = ball_kickpoint.distanceTo(obstP);
			qreal dotproduct = ball_kickpoint.dx()*ball_obst.dx() + ball_kickpoint.dy()*ball_obst.dy();
			if(dist<obst->body().radius() && dotproduct>0)
				return false;
	}
	team = this->robot()->enemyTeam();
	for(int i=0; i<team->size(); i++){
		Robot* obst = team->at(i);
			Point obstP(obst->x(), obst->y());
			Line ball_obst = Line(*ball, obstP);
			qreal dist = ball_kickpoint.distanceTo(obstP);
			qreal dotproduct = ball_kickpoint.dx()*ball_obst.dx() + ball_kickpoint.dy()*ball_obst.dy();
			if(dist<obst->body().radius() && dotproduct>0)
				return false;
	}
	return true;
}

bool DriveToDribbleT::condition()
{
	//Zickler43* z = (Zickler43*) this->parent();//SA: Dead store
	return !source_->busy(); //Vector(*z->robot() - *z->stage()->ball()).length() < MINDIST;//
}

DriveToDribbleT::DriveToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToDriveT::condition()
{
	//SampledDribble* dribble = (SampledDribble*)source_;
	//DriveToBall* drive = (DriveToBall*)target_;
	//const Object* backup = drive->getRefLookPoint();
	//drive->setRefLookPoint(dribble->getRefLookPoint());
	//bool busy = drive->busy();
	//drive->setRefLookPoint(backup);
	//return busy;
	//Zickler43* z = (Zickler43*) this->parent();//SA: Dead store
	return source_->busy();//Vector(*z->robot() - *z->stage()->ball()).length() >= MINDIST;//!source_->busy(); //
}

DribbleToDriveT::DribbleToDriveT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToDribbleT::condition()
{
	return !source_->busy();
}

DribbleToDribbleT::DribbleToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToGoalKickT::condition()
{
    Zickler43* z = (Zickler43*) this->parent();
    return !source_->busy() && z->holeSize()>0;
}

DribbleToGoalKickT::DribbleToGoalKickT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool GoalKickToDriveT::condition()
{
	return source_->busy();
}

GoalKickToDriveT::GoalKickToDriveT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

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
