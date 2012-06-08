#include "Goto.h"
#include "ForceFieldMotion.h"
#include "Robot.h"
#include "PID.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include <QPoint>
#include <QLine>
#include "mathutils.h"

using namespace LibIntelligence;
using namespace Skills;

#define REP 10.00
#define ATT 0.001

bool isInDefenseArea(qreal x, qreal y, qreal r, qreal x_goal, qreal s)
{
	qreal x0 = x_goal - x;
	if(x_goal > 0)
		if(x > x_goal - r)
			if(y<s/2 + sqrt(r*r-x0*x0) && y>-s/2-sqrt(r*r-x0*x0))	
				return true;
	else
		if(x < x_goal + r)
			if(y<s/2 + sqrt(r*r-x0*x0) && y>-s/2-sqrt(r*r-x0*x0))	
				return true;
		return false;
}

QPointF awayFromDefensePoint(qreal x, qreal y, qreal r, qreal x_goal, qreal s)
{
	QPointF result = QPointF(0, 0);
	if(x_goal > 0)
	{
		QLineF area_line = QLineF(x_goal - r , s/2, x_goal -r, -s/2);
		QLineF join_to_goal = QLineF(x, y, x_goal, 0);
	    
		join_to_goal.intersect(area_line, &result);
		if(result.y() > -s/2 && result.y()< s/2) return result;
		
		else
		{
			qreal dist_goal = sqrt((x-x_goal)*(x-x_goal) + y*y);
			qreal sin_o = y/dist_goal, cos_o = (x-x_goal)/dist_goal;
			result.setX(x_goal + r*cos_o);
			result.setY(r*sin_o);
			return result;
		}
	}
	else
	{
		QLineF area_line = QLineF(x_goal - r , s/2, x_goal -r, -s/2);
		QLineF join_to_goal = QLineF(x, y, x_goal, 0);
	    
		join_to_goal.intersect(area_line, &result);
		if(result.y() > -s / 2 && result.y() < s / 2) return result;
		
		else
		{
			qreal dist_goal = sqrt((x - x_goal) * (x - x_goal) + y * y);
			qreal sin_o = y / dist_goal, cos_o = (x - x_goal)/dist_goal;
			result.setX(x_goal + r * cos_o);
			result.setY(r * sin_o);
			return result;
		}
	}
}



Goto::Goto(QObject* p, Robot* r, qreal x, qreal y, qreal o, qreal s, bool a)
	: ForceFieldMotion(p, r, x, y, o, s, REP, ATT), ignoreBrake(false), allowDefenseArea(a) {}

void Goto::setAll(qreal x, qreal y, qreal s)
{
	ForceFieldMotion::setAll(x, y, s, REP, ATT);
}

void Goto::setIgnoreBrake()
{
	ignoreBrake = true;
}

void Goto::setNotIgnoreBrake()
{
	ignoreBrake = false;
}

void Goto::setAllowDefenseArea()
{
	allowDefenseArea = true;
}

void Goto::setNotAllowDefenseArea()
{
	allowDefenseArea = false;
}

void Goto::step()
{
	//TODO: valores objetivos devem ser alterados para valor nao deterministico (soma um float a speedx speedy e speedang)

	qreal dx, dy, d, targetTempX, targetTempY;

	targetTempX = targetX;
	targetTempY = targetY;

	//limitar dentro do campo
	qreal limit = robot()->body().radius() + stage()->ball()->radius();
	qreal maxX = stage()->fieldLength() / 2 + limit;
	qreal minX = -maxX;
	qreal maxY = stage()->fieldWidth() / 2 + limit;
	qreal minY = -maxY;

	targetX = targetX < minX ? minX : targetX > maxX ? maxX : targetX;
	targetY = targetY < minY ? minY : targetY > maxY ? maxY : targetY;

	//limitar fora da area

	if(!allowDefenseArea) {
		if(isInDefenseArea(targetX, targetY, stage()->defenseRadius() + robot()->body().radius(), robot()->goal()->x(), stage()->defenseStretch() + robot()->body().radius()))
		{
			QPointF new_pos = awayFromDefensePoint(targetX, targetY, stage()->defenseRadius() + robot()->body().radius(), robot()->goal()->x(), stage()->defenseStretch() + robot()->body().radius());
			targetX = new_pos.x();
			targetY = new_pos.y();
		}
	}

	dx = targetX - robot()->x();
	dy = targetY - robot()->y();
	d = sqrt(dx * dx + dy * dy);

	qreal temp = speed;

	if(!ignoreBrake) {
		qreal distance = sqrt((robot()->x() - targetX) * (robot()->x() - targetX) + (robot()->y() - targetY) * (robot()->y() - targetY));
		qreal s = sqrt(2 * 1000 * distance);
		s = s < speed ? s : speed;
		//ForceFieldMotion::setSpeed(speed * distance / 1000 < speed ? speed * distance / 1000 : speed);
		ForceFieldMotion::setSpeed(s);
	}

	CONTROLLER_S controlller(/*4.0*/1.8, 0.0, 0.0, 12.0, 2.0);//valores carteados
	controlller.entrada = __q(orientation - robot()->orientation());
	controlller.realimentacao = 0.0;
	pidService(controlller);

	//ForceFieldMotion::setSpeed(sqrt(temp * temp + a * distance));

	ForceFieldMotion::step();

	//Restaura valor setSpeed passado pelo construtor
	ForceFieldMotion::setSpeed(temp); 

	//Restaura targetX, targetY
	targetX = targetTempX;
	targetY = targetTempY;

	//if(d > 2 * robot()->body().radius()) ForceFieldMotion::step();
	//else {
	//	/*CONTROLLER_S conx(1.5, 0.0, 0.0, ROBOT_MAX_LINEAR_SPEED, 5.0);
	//	conx.entrada = targetX;
	//	conx.realimentacao = robot()->x();
	//	pidService(conx);
	//	dx = conx.saida;
	//	CONTROLLER_S cony(1.5, 0.0, 0.0, ROBOT_MAX_LINEAR_SPEED, 5.0);
	//	conx.entrada = targetY;
	//	conx.realimentacao = robot()->y();
	//	pidService(cony);
	//	dy = cony.saida;*/
	//	dx /= robot()->body().radius() * robot()->body().radius();
	//	dy /= robot()->body().radius() * robot()->body().radius();
	//	Steer::setSpeeds(speed * dx, speed * dy);
	//	Steer::step();
	//}

	//TODO: ReachedSpeedX, ReachedSpeedY, ReachedSpeedAng
	//TODO: mudar flag busy
	//TODO: descobrir onde dou o emit, eh na skill de mais alto nivel ou na de mais baixo nivel???
	//TODO: 
}