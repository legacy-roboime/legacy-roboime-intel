#include "AttackerMinMax2.h"
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
using namespace AttackerMinMax2T;

AttackerMinMax2::AttackerMinMax2(QObject* p, Robot* r, qreal speed, qreal dribbleSpeed, qreal passSpeed)
	: Tactic(p, r, true),
	hasKick_(false),
	hasPass_(false)
{
	movePoint_ = new Object();
	kickPoint_ = new Object();
	dribblePoint_ = new Object();
	//driveToBall_ = new DriveToBall(this, r, r->enemyGoal(), speed, true);
	dribble_ = new SampledDribble(this, r, dribblePoint_, true, 1., 1., dribbleSpeed);
	goalKick_ = new SampledKick(this, r, kickPoint_, true, 1., 1., dribbleSpeed, false);
	pass_ = new SampledKick(this, r, kickPoint_, true, 0.2, 0.2, dribbleSpeed, false);
	goto_ = new Goto(this, r, movePoint_->x(), movePoint_->y(), 0, speed, false);
	this->speed = speed;

	//this->pushState(driveToBall_); //this is important to destructor
	this->pushState(goalKick_);
	this->pushState(dribble_);
	this->pushState(pass_);
	this->pushState(goto_);

	//driveToBall_->setObjectName("DriveToBall");
	dribble_->setObjectName("SampledDribble");
	goalKick_->setObjectName("SampledGoalKick");
	pass_->setObjectName("SampledPass");
	goto_->setObjectName("Goto");

	this->pushTransition(goto_, new GotoToDriveT(this, goto_, /*driveToBall_*/dribble_));
	this->pushTransition(/*driveToBall_*/dribble_, new DriveToGotoT(this, /*driveToBall_*/dribble_, goto_));
	//this->pushTransition(driveToBall_, new DriveToDribbleT(this, driveToBall_, dribble_));
	//this->pushTransition(dribble_, new DribbleToDriveT(this, dribble_, driveToBall_));
	this->pushTransition(dribble_, new DribbleToGoalKickT(this, dribble_, goalKick_));
	this->pushTransition(goalKick_, new GoalKickToDribbleT(this, goalKick_, dribble_));
	this->pushTransition(dribble_, new DribbleToPassT(this, dribble_, pass_));
	this->pushTransition(pass_, new PassToDribbleT(this, pass_, dribble_));

	this->setInitialState(goto_);

	this->reset();
}

AttackerMinMax2::~AttackerMinMax2()
{
	//delete driveToBall_;
	delete dribble_;
	delete goalKick_;
	delete pass_;
	delete goto_;
	delete movePoint_;
	delete kickPoint_;
}

SampledDribble* AttackerMinMax2::dribble()
{
	return dribble_;
}

Object* AttackerMinMax2::kickPoint()
{
	return kickPoint_;
}

Object* AttackerMinMax2::movePoint()
{
	return movePoint_;
}

bool AttackerMinMax2::hasKick()
{
	return hasKick_;
}

bool AttackerMinMax2::hasPass()
{
	return hasPass_;
}

void AttackerMinMax2::updateSoccerAction(bool hasKick, bool hasPass, bool getBall, qreal kickPointX, qreal kickPointY, qreal movePointX, qreal movePointY)
{
	Robot* robot = this->robot();
	Ball* ball = this->stage()->ball();
	Goal* enemyGoal = robot->enemyGoal();

	//if(movePointX == ball->x() && movePointY == ball->y()){
		//movePointX = enemyGoal->x();
		//movePointY = enemyGoal->y();
		//cout << "BUGADDOO" << endl;
	//}

	hasKick_ = hasKick;
	hasPass_ = hasPass;
	if(hasKick || hasPass){
		kickPoint_->setX(kickPointX);
		kickPoint_->setY(kickPointY);
	}

	if(getBall && robot->distance(ball).module() < MIN_DIST){
		movePoint_->setX(enemyGoal->x());
		movePoint_->setY(enemyGoal->y());
	}
	else{
		movePoint_->setX(movePointX);
		movePoint_->setY(movePointY);
	}

	QLineF line = QLineF(robot->x(), robot->y(), ball->x(), ball->y());
	qreal orientation = 2 * M_PI - line.angle() * M_PI / 180.;
	goto_->setPoint(movePoint_->x(), movePoint_->y());
	goto_->setOrientation(orientation);

	dribblePoint_->setX(movePointX - robot->x() + ball->x());
	dribblePoint_->setY(movePointY - robot->y() + ball->y());
}

bool GotoToDriveT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	Robot* r = a->robot();
	Ball* b = a->stage()->ball();
	return r->distance(b).module() < MIN_DIST;
}

GotoToDriveT::GotoToDriveT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DriveToGotoT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	Robot* r = a->robot();
	Ball* b = a->stage()->ball();
	return !(r->distance(b).module() < MIN_DIST);
}

DriveToGotoT::DriveToGotoT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DriveToDribbleT::condition()
{
	return !source_->busy(); 
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

bool DribbleToGoalKickT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return !source_->busy() && a->hasKick(); 
}

DribbleToGoalKickT::DribbleToGoalKickT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool GoalKickToDribbleT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	//QLineF line = QLineF(a->robot()->x(), a->robot()->y(), a->stage()->ball()->x(), a->stage()->ball()->y()); 
	//qreal angle2 = 360 - line.angle();
	//cout << "VOLTA " <<  angle2 << endl;
	return !source_->busy() && !a->hasKick(); 
}

GoalKickToDribbleT::GoalKickToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToPassT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return !source_->busy() && a->hasPass();
}

DribbleToPassT::DribbleToPassT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool PassToDribbleT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return !source_->busy() && !a->hasPass();
}

PassToDribbleT::PassToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DefaultTrueT::condition() 
{
	return true;
}

DefaultTrueT::DefaultTrueT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}