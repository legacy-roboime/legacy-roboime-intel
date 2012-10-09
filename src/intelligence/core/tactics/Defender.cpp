#include "Defender.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include "Team.h"
#include <cmath>
#include "geomutils.h"

#define MINDIST 3000

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;
using namespace DefenderT;

Defender::Defender(QObject* p, Robot* r, Object* enemy, Point* cover, qreal dist, qreal speed)
	: Tactic(p,r),
	enemy_(enemy),
	driveToObj(new DriveToObject(this, r, (const Object*) cover, -(dist + r->body().cut()), enemy_, 100, 0.1745329251, speed, true)),
	fac(new FollowAndCover(this, r, (Point*) enemy_, cover, 300, speed))
{
	this->pushState(driveToObj);
	this->pushState(fac);

	driveToObj->setObjectName("DriveToObject");
	fac->setObjectName("FollowAndCover");

	this->pushTransition(driveToObj, new DriveObjToFacT(this, driveToObj, fac, 1.));
	this->pushTransition(fac, new FacToDriveObjT(this, fac, driveToObj, 1.));

	this->setInitialState(driveToObj);

	this->reset();
}

void Defender::setEnemy(Object* enemy)
{
	enemy_ = enemy;
	driveToObj->setRefLookPoint(enemy);
	fac->setFollow(enemy);
}

void Defender::follow()
{
	this->setCurrentState(fac);
}

Object* Defender::enemy()
{
	return enemy_;
}

bool DriveObjToFacT::condition()
{
	Defender* d = (Defender*) this->parent();
	return QVector2D(*d->enemy() - *d->robot()->goal()).length() < MINDIST;
}

DriveObjToFacT::DriveObjToFacT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

bool FacToDriveObjT::condition()
{
	Defender* d = (Defender*) this->parent();
	return QVector2D(*d->enemy() - *d->robot()->goal()).length() >= MINDIST;
}

FacToDriveObjT::FacToDriveObjT(QObject* parent, State* source, State* target, qreal probability) : MachineTransition(parent, source, target, probability){}

//Tática do Zagueiro
// Se não pode chutar a bola, fica na mesma coordenada Y da bola
//void Defender::step()
//{
//	Stage* stage = this->stage();
//	Robot* robot = this->robot();
//	goto_->step();
//	Goal* myGoal;
//	Ball* ball = stage->ball();
//
//	myGoal = robot->goal();
//	//Gambiarra:
//	//myGoal->setX(-3000.0);
//
//
//	qreal X;
//	qreal Y;
//
//	qreal RobotRadius = 50;
//	
//	//Parametrizacao para os dois lados do campo
//	int mod;
//	if(robot->goal()->x() < 0 )	mod = +1;
//	else						mod = -1;
//
//	qreal radius = stage->defenseRadius() + RobotRadius;
//	if(position_ == -1){
//
//		X = robot->goal()->x() + mod * radius * sqrt(2.0)/2;
//		Y = robot->goal()->y() + stage->defenseStretch()/2 + radius * sqrt(2.0)/2;
//	}
//	else if(position_ == 0 ) {
//		X = robot->goal()->x() + mod * radius;
//		Y = robot->goal()->y();
//	}
//	else if(position_ ==1) {
//		X = robot->goal()->x() + mod * radius * sqrt(2.0)/2;
//		Y = robot->goal()->y() - stage->defenseStretch()/2 - radius * sqrt(2.0)/2;
//	}
//
//
//	goto_->setPoint(X,Y);
//	
//
//	goto_->setOrientation(ball->x() - robot->x(), ball->y() - robot->y());
//	goto_->step();
//
//
//}


//void Defender::step()
//{
//	Stage* stage = this->stage();
//	Robot* robot = this->robot();
//	goto_->step();
//	Goal* myGoal;
//	Ball* ball = stage->ball();
//
//	myGoal = robot->goal();
//
//	QLineF bolaGol(myGoal->x(), myGoal->y(), ball->x(), ball->y());
//
//	//Nova versao
//	Robot* Inimigo = stage->getClosestPlayerToBall(robot->enemyTeam());
//	qreal IniX = Inimigo->x();
//	qreal IniY = Inimigo->y();
//
//	//Coordenadas do goleiro
//	qreal GolieX = robot->team()->at(2)->x();
//	qreal GolieY = robot->team()->at(2)->y();
//
//	//Reta entre o inimigo e a bola
//	QLineF IniBallLine = QLineF(ball->x(), ball->y(), IniX, IniY);
//
//	//Reta de posicao do goleiro
//	qreal displace = myGoal->x() > 0 ? -600.0 : 600.0; //Distancia entre a posicao do goleiro e o gol
//	QLineF GoalLine = QLineF(myGoal->x() + displace, 0.0, myGoal->x() + displace, 1.0);
//
//
//	QPointF GoliePos;
//	GoalLine.intersect(IniBallLine, &GoliePos);
//
//	qreal goalWidth = myGoal->width();
//	qreal goalY = myGoal->y();
//	qreal robotY = GoliePos.y();
//
//	QLineF ballGoalLine = QLineF(ball->x(),ball->y(),robot->goal()->x(),robot->goal()->y());
//	if(robotY > goalY + goalWidth/2) {
//		ballGoalLine.intersect(GoalLine, &GoliePos);
//		//GoliePos.setY(goalY + goalWidth/2);
//	}
//	else if(robotY < goalY - goalWidth/2)
//	{
//		ballGoalLine.intersect(GoalLine, &GoliePos);
//		//GoliePos.setY(goalY - goalWidth/2);
//	}
//
//	robotY = GoliePos.y();
//	if(robotY > goalY + goalWidth/2) {
//		//ballGoalLine.intersect(GoalLine, &GoliePos);
//		GoliePos.setY(goalY + goalWidth/2);
//
//		if(stage->ball()->y() * robot->y() < 0)  GoliePos.setY(goalY - goalWidth/2);
//	}
//	else if(robotY < goalY - goalWidth/2)
//	{
//		//ballGoalLine.intersect(GoalLine, &GoliePos);
//		GoliePos.setY(goalY - goalWidth/2);
//		if(stage->ball()->y() * robot->y() < 0)  GoliePos.setY(goalY + goalWidth/2);
//	}
//	/*
//	if(GoliePos.y() > myGoal->width()/2 + myGoal->y()) 
//		GoliePos.setY(myGoal->width()/2 + myGoal->y());
//
//	else if(GoliePos.y() < myGoal->y() - myGoal->width()/2)
//		GoliePos.setY(myGoal->y() - myGoal->width()/2);
//*/
//
//	goto_->setOrientation(IniX - robot->x(), IniY - robot->y());
//	//goto_->setPoint(GoliePos.x(), GoliePos.y());
//	goto_->setPoint(GoliePos.x(), GoliePos.y());
//	goto_->step();
//
//
//	/* Modificacao anterior
//	qreal raio = myGoal->width() / 2;
//
//	QPointF ponto(myGoal->x() - (myGoal->x() - ball->x()) * raio / bolaGol.length(), - (myGoal->y() - ball->y()) * raio / bolaGol.length());
//
//	goto_->setOrientation(bolaGol.dx(), bolaGol.dy());
//	goto_->setPoint(ponto.x(), ponto.y());
//	goto_->step();
//
//	*/
//	/*
//	//Gambiarra:
//	//TODO: corrigir valores de tamanho e posicao do gol
//	//myGoal->setX(-3000.0);
//	//myGoal->setWidth(
//
//	//Coordenadas do robo inimigo
//	//Robot* Inimigo = stage->getCloserPlayerToBall(robot->enemyTeam());
//	Robot* Inimigo = stage->getCloserPlayerToBall(robot->team());
//	qreal IniX = Inimigo->x();
//	qreal IniY = Inimigo->y();
//
//	//Coordenadas do goleiro
//	qreal GolieX = robot->team()->at(2)->x();
//	qreal GolieY = robot->team()->at(2)->y();
//
//	//Reta entre a bola e o jogador
//	QLineF IniBallLine = QLineF(ball->x(), ball->y(), IniX, IniY);
//	//QLineF IniBallLine = QLineF(ball->x(), ball->y(), myGoal->x(), 0.0);
//
//	//Reta do gol
//
//	qreal displace = myGoal->x() > 0 ? -300.0 : 300.0;
//	QLineF GoalLine = QLineF(myGoal->x() + displace, 0.0, myGoal->x() + displace, 1.0);
//
//
//	QPointF GoliePos;
//	GoalLine.intersect(IniBallLine, &GoliePos);
//
//
//	if(GoliePos.y() > myGoal->width()/2 + myGoal->y()) 
//		GoliePos.setY(myGoal->width()/2 + myGoal->y());
//
//	else if(GoliePos.y() < myGoal->y() - myGoal->width()/2)
//		GoliePos.setY(myGoal->y() - myGoal->width()/2);
//
//	//make goalkeeper position
//	qreal ratio = myGoal->width() / 2 / sqrt((stage->ball()->x() - myGoal->x()) * (stage->ball()->x() - myGoal->x()) + (stage->ball()->y() - myGoal->y()) * (stage->ball()->y() - myGoal->y()));
//	//qreal gx = (stage->ball()->x() - myGoal->x()) * ratio + myGoal->x();
//	//qreal gy = (stage->ball()->y() - myGoal->y()) * ratio + myGoal->y();
//	GoalLine = QLineF(myGoal->x(), myGoal->y(), stage->ball()->x(), stage->ball()->y());
//	qreal gx = - GoalLine.normalVector().dx() * ratio + myGoal->x();
//	qreal gy = - GoalLine.normalVector().dy() * ratio + myGoal->y();
//
//	goto_->setOrientation(IniX - robot->x(), IniY - robot->y());
//	//goto_->setPoint(GoliePos.x(), GoliePos.y());
//	goto_->setPoint(gx, gy);
//	goto_->step();*/
//}
