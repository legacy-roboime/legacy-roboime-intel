#include "AttackerMinMax2.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include "Sampler.h"
#include <iostream>
#include "mathutils.h"
#include "soccer.h"

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;
using namespace AttackerMinMax2T;

AttackerMinMax2::AttackerMinMax2(QObject* p, Robot* r, Object* kickPoint, Object* movePoint, Object* dribblePoint, qreal speed, qreal dribbleSpeed, qreal passSpeed)
	: Tactic(p, r, true)
{
	action_ = null_action;

	if(movePoint == NULL)
		movePoint_ = new Object();
	else
		movePoint_ = movePoint;
	if(kickPoint == NULL)
		kickPoint_ = new Object();
	else
		kickPoint_ = kickPoint;
	if(dribblePoint == NULL)
		dribblePoint_ = new Object();
	else
		dribblePoint_ = dribblePoint;

	driveToBall_ = new DriveToBall(this, r, dribblePoint_, speed, true);
	dribble_ = new SampledDribble(this, r, dribblePoint_, true, 1., 1., speed);
	goalKick_ = new SampledKick(this, r, kickPoint_, true, 1., 1., speed, false);
	pass_ = new SampledKick(this, r, kickPoint_, true, 0.4, 0.4, speed, true);
	goto_ = new Goto(this, r, movePoint_->x(), movePoint_->y(), 0, speed, false);

	driveToBall_->setAllowDefenseArea();
	dribble_->setAllowDefenseArea();
	goalKick_->setAllowDefenseArea();
	pass_->setAllowDefenseArea();
	goto_->setAllowDefenseArea();

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
	this->pushTransition(dribble_, new DribbleToGoalKickT(this, dribble_, goalKick_));
	this->pushTransition(dribble_, new DribbleToPassT(this, dribble_, pass_));
	this->pushTransition(dribble_, new DribbleToDriveT(this, dribble_, driveToBall_));
	this->pushTransition(goalKick_, new GoalKickToDribbleT(this, goalKick_, dribble_));
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

type_actions AttackerMinMax2::action()
{
	return action_;
}

//atualiza maquina (tatica) que controla o owner
void AttackerMinMax2::updateSoccerAction(type_actions action, Vector2 kickPoint, Vector2 movePoint)
{
	action_ = action;

	Robot* robot = this->robot();
	Ball* ball = this->stage()->ball();
	Goal* enemyGoal = robot->enemyGoal();

	Line move_ball = Line(*ball, Point(movePoint.x, movePoint.y));

	//forcei transicao na maquina de estado para simplificar, evitar colocar mais MachineTransition
	if(action == get_ball){//owner pegando a bola
		//adicionei a heuristica do look point, pois o soccer nao define
		dribblePoint_->setX(enemyGoal->x());
		dribblePoint_->setY(enemyGoal->y());

		this->setCurrentState(driveToBall_);
	}
	else if(action == kick_to_goal){//owner chutando ao gol
		kickPoint_->setX(kickPoint.x);
		kickPoint_->setY(kickPoint.y);

		this->setCurrentState(goalKick_);
	}
	else if(action == pass){//owner passando
		kickPoint_->setX(kickPoint.x);
		kickPoint_->setY(kickPoint.y);

		this->setCurrentState(pass_);
	}
	else if(action == ::move && move_ball.length() > MIN_DIST){//owner conduzindo bola ate movePoint
		//TODO: colocar uma condição para ele parar de conduzir quando chegar no movePoint

		dribblePoint_->setX(movePoint.x);
		dribblePoint_->setY(movePoint.y);

		this->setCurrentState(dribble_);
	}
	else if(action == ::move){
		//Mantem conduzindo para o ultimo ponto de dribblePoint_

		this->setCurrentState(dribble_);
	}
	else if(action == blocker){//owner vai para movePoint
		Line line = Line(robot->x(), robot->y(), ball->x(), ball->y());
		qreal orientation = line.angle() * M_PI / 180.;
		goto_->setPoint(movePoint.x, movePoint.y);
		goto_->setOrientation(orientation);

		this->setCurrentState(goto_);
	}
	else if(action == move_table){//owner vai para movePoint
		Line line = Line(robot->x(), robot->y(), ball->x(), ball->y());
		qreal orientation = line.angle() * M_PI / 180.;
		goto_->setPoint(movePoint.x, movePoint.y);
		goto_->setOrientation(orientation);

		this->setCurrentState(goto_);
	}
	else if(action == receive_ball){//owner fazendo move_table, vai para movePoint
		Line line = Line(robot->x(), robot->y(), ball->x(), ball->y());
		qreal orientation = line.angle() * M_PI / 180.;
		goto_->setPoint(movePoint.x, movePoint.y);
		goto_->setOrientation(orientation);

		this->setCurrentState(goto_);
	}
	else if(action == null_action){//owner vai para movePoint
		Line line = Line(robot->x(), robot->y(), ball->x(), ball->y());
		qreal orientation = line.angle() * M_PI / 180.;
		goto_->setPoint(movePoint.x, movePoint.y);
		goto_->setOrientation(orientation);

		this->setCurrentState(goto_);
	}
}

void AttackerMinMax2::setSpeed(qreal speed)
{
	this->driveToBall_->setSpeed(speed);
	this->dribble_->setSpeed(speed);
	this->goalKick_->setSpeed(speed);
	this->pass_->setSpeed(speed);
	this->goto_->setSpeed(speed);
}

bool GotoToDriveT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return Vector(*a->robot() - *a->stage()->ball()).length() < MIN_DIST;
}

GotoToDriveT::GotoToDriveT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DriveToGotoT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return Vector(*a->robot() - *a->stage()->ball()).length() >= MIN_DIST;
}

DriveToGotoT::DriveToGotoT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DriveToDribbleT::condition()
{
	return !source_->busy(); 
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
	return source_->busy();//busy;
}

DribbleToDriveT::DribbleToDriveT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToGoalKickT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return /*!source_->busy() &&*/ a->action() == kick_to_goal; 
}

DribbleToGoalKickT::DribbleToGoalKickT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool GoalKickToDribbleT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	
	return /*QVector2D(*a->robot() - *a->stage()->ball()).length() >= MIN_DIST &&*/ !(a->action() == kick_to_goal); 
}

GoalKickToDribbleT::GoalKickToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DribbleToPassT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return /*!source_->busy() &&*/ a->action() == pass;
}

DribbleToPassT::DribbleToPassT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool PassToDribbleT::condition()
{
	AttackerMinMax2* a = (AttackerMinMax2*)this->parent();
	return /*QVector2D(*a->robot() - *a->stage()->ball()).length() >= MIN_DIST &&*/ !(a->action() == pass);
}

PassToDribbleT::PassToDribbleT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool DefaultTrueT::condition() 
{
	return true;
}

DefaultTrueT::DefaultTrueT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}
