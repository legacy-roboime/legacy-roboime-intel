#include "Goto.h"
#include "ForceFieldMotion.h"
#include "Robot.h"
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
#define LIMERR 2

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
	: Steer(p, r, 0, 0, o),
	ignoreBrake(false), 
	allowDefenseArea(a), 
	targetX(x),
	targetY(y),
	speed(s),
	controllerSpeedX(2, 0, 0.0, M_INF, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, M_INF, LIMERR)//valores carteados
{}

void Goto::setAll(qreal x, qreal y, qreal s)
{

}

void Goto::setPoint(qreal _x, qreal _y)
{
	targetX = _x;
	targetY = _y;
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

void Goto::setSpeed(qreal s)
{
	this->speed = s;
	//controllerSpeedX.saidaMax = controllerSpeedY.saidaMax = s;
}

void Goto::setPIDkGoto(double kp, double ki, double kd)
{
	controllerSpeedX.Kp = controllerSpeedY.Kp = kp;
	controllerSpeedX.Ki = controllerSpeedY.Ki = ki;
	controllerSpeedX.Kd = controllerSpeedY.Kd = kd;
}

void Goto::printPIDk()
{
	cout << controllerSpeedX.Kp << " " << controllerSpeedX.Ki << " " << controllerSpeedX.Kd << endl;
}
void Goto::step()
{
	//TODO: valores objetivos devem ser alterados para valor nao deterministico (soma um float a speedx speedy e speedang)

	qreal targetTempX, targetTempY, speedX, speedY, speedTemp, k;
	speedX = speedY = 0;
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

	if(!ignoreBrake) {

		//controle speedX

		controllerSpeedX.entrada = targetX; //deseja-se que a distância entre o alvo e o robô seja igual a zero
		qreal x = robot()->x();
		controllerSpeedX.realimentacao = robot()->x();
		pidService(controllerSpeedX);
		speedX = controllerSpeedX.saida;

		//controle speedY

		controllerSpeedY.entrada = targetY; //deseja-se que a distância entre o alvo e o robô seja igual a zero
		qreal y = robot()->y();
		controllerSpeedY.realimentacao = robot()->y();
		pidService(controllerSpeedY);
		speedY = controllerSpeedY.saida;

		//calculo speed linear
		speedTemp = sqrt(speedX*speedX + speedY*speedY);
		if(speedTemp > speed)
		{
			k = speed/speedTemp;
			speedX *= k;
			speedY *= k;
		}
	}

	Steer::setSpeeds(speedX, speedY);
	Steer::step();

	//restaura targetX, targetY
	targetX = targetTempX;
	targetY = targetTempY;
}