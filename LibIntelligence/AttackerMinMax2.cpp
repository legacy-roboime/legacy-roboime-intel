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
	driveToBall_ = new DriveToBall(this, r, r->enemyGoal(), speed, true);
	dribble_ = new SampledDribble(this, r, movePoint_, true, 0., 1., dribbleSpeed);
	goalKick_ = new SampledKick(this, r, kickPoint_, true, 0.9, 1., dribbleSpeed, false);
	pass_ = new SampledKick(this, r, kickPoint_, true, passSpeed, passSpeed, dribbleSpeed, false);
	goto_ = new Goto(this, r, movePoint_->x(), movePoint_->y(), 0, speed, false);
	speed = speed;

	this->pushState(driveToBall_); //this is important to destructor
	this->pushState(goalKick_);
	this->pushState(dribble_);
	this->pushState(pass_);
	this->pushState(goto_);

	driveToBall_->setObjectName("DriveToBall");
	dribble_->setObjectName("SampledDribble");
	goalKick_->setObjectName("SampledGoalKick");
	pass_->setObjectName("SampledPass");
	goto_->setObjectName("Goto");

	this->pushTransition(goto_, new GotoToDriveT(this, goto_, driveToBall_));
	this->pushTransition(driveToBall_, new DriveToGotoT(this, driveToBall_, goto_));
	this->pushTransition(driveToBall_, new DriveToDribbleT(this, driveToBall_, dribble_));
	this->pushTransition(dribble_, new DribbleToDriveT(this, dribble_, driveToBall_));
	this->pushTransition(dribble_, new DribbleToGoalKickT(this, dribble_, goalKick_));
	this->pushTransition(goalKick_, new GoalKickToDribbleT(this, goalKick_, dribble_));
	this->pushTransition(dribble_, new DribbleToPassT(this, dribble_, pass_));
	this->pushTransition(pass_, new PassToDribbleT(this, pass_, dribble_));

	this->setInitialState(goto_);

	this->reset();
}

AttackerMinMax2::~AttackerMinMax2()
{
	delete driveToBall_;
	delete dribble_;
	delete goalKick_;
	delete pass_;
	delete goto_;
	delete movePoint_;
	delete kickPoint_;
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

void AttackerMinMax2::updateSoccerAction(bool hasKick, bool hasPass, qreal kickPointX, qreal kickPointY, qreal movePointX, qreal movePointY)
{
	hasKick_ = hasKick;
	hasPass_ = hasPass;
	kickPoint_->setX(kickPointX);
	kickPoint_->setY(kickPointY);
	movePoint_->setX(movePointX);
	movePoint_->setY(movePointY);
}

void AttackerMinMax2::step()
{
	Skill* current = (Skill*)this->getCurrentState();
	this->execute();
	current->step();
	//cout << current->objectName().toStdString() << endl;
}

bool AttackerMinMax2::getMinDist()
{
	Robot* r = robot();
	Ball* ball = r->stage()->ball();
	return ( r->distance(ball).module() < 500 ); 
}

//lembrar q a condition nao eh obrigatoriamente relacionado com o busy(), isso eh soh um reuso de codigo
bool GotoToDriveT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return a->getMinDist();
}

GotoToDriveT::GotoToDriveT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DriveToGotoT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return !a->getMinDist();
}

DriveToGotoT::DriveToGotoT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DriveToDribbleT::condition()
{
	return !source_->busy(); 
}

DriveToDribbleT::DriveToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToDriveT::condition()
{
	return source_->busy(); 
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
	return !a->hasKick(); 
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
	return !a->hasPass();
}

PassToDribbleT::PassToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DefaultTrueT::condition() 
{
	return true;
}

DefaultTrueT::DefaultTrueT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}