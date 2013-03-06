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

#define LIMERR 2
#define MAXCSPEED 20000
#define AREADISC 5

Point awayFromDefenseArea(const Point &point, const Robot *r, const Point *g)
{
	Stage *s(r->stage());
	qreal halfStretch(s->defenseStretch() / 2);
	qreal radius(r->body().radius() + s->defenseRadius());
	// this is the stretch line over the goal
	Line base(g->x(), g->y() - halfStretch, g->x(), g->y() + halfStretch);

	//inside the rectangle
	if(point.y() >= base.p1().y() && point.y() <= base.p2().y() && fabs(point.x() - g->x()) < radius) {
		//the defense line is the line which must not be crossed
		Line defenseLine(base.translated(g->x() > 0 ? -radius : radius, 0));
		return Point(defenseLine.p1().x(), point.y());

	} else if(Line(point, base.p1()).length() < radius) {
		Line line(base.p1(), point);
		line.setLength(radius);
		return line.p2();

	} else if(Line(point, base.p2()).length() < radius) {
		Line line(base.p2(), point);
		line.setLength(radius);
		return line.p2();
	} else {
		// this is not suposed to be reached on normal flow
		return Point();
	}
}

bool isInDefenseArea(const Point &point, const Robot *r, const Point *g)
{
	Stage *s(r->stage());
	qreal halfStretch(s->defenseStretch() / 2);
	qreal radius(r->body().radius() + s->defenseRadius());
	// this is the stretch line over the goal
	Line base(g->x(), g->y() - halfStretch, g->x(), g->y() + halfStretch);

	//inside the rectangle
	return (point.y() >= base.p1().y() && point.y() <= base.p2().y() && abs(point.x() - g->x()) < radius)
		|| (Line(point, base.p1()).length() < radius)
		|| (Line(point, base.p2()).length() < radius);
}

Goto::Goto(QObject* p, Robot* r, Point *t, qreal o, qreal s, bool a)
	: Steer(p, r, 0, 0, o),
	pTarget(t),
	old(0.0, 0.0),
	lookAt(NULL),
	ignoreBrake(false),
	allowDefenseArea(a),
	speed(s),
	controllerSpeedX(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	plusSpeed_(0, 0)
{}

Goto::Goto(QObject* p, Robot* r, Point t, Point *l, qreal s, bool a)
	: Steer(p, r, 0, 0, 0),
	pTarget(NULL),
	target(t),
	old(0.0, 0.0),
	lookAt(l),
	ignoreBrake(false),
	allowDefenseArea(a),
	speed(s),
	controllerSpeedX(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	plusSpeed_(0, 0)
{}

Goto::Goto(QObject* p, Robot* r, Point *t, Point *l, qreal s, bool a)
	: Steer(p, r, 0, 0, 0),
	pTarget(t),
	old(0.0, 0.0),
	lookAt(l),
	ignoreBrake(false),
	allowDefenseArea(a),
	speed(s),
	controllerSpeedX(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	plusSpeed_(0, 0)
{}

Goto::Goto(QObject* p, Robot* r, qreal x, qreal y, qreal o, qreal s, bool a)
	: Steer(p, r, 0, 0, o),
	pTarget(NULL),
	target(0, 0),
	old(0.0, 0.0),
	lookAt(NULL),
	ignoreBrake(false), 
	allowDefenseArea(a), 
	speed(s),
	controllerSpeedX(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	plusSpeed_(0, 0)
{}

Goto::~Goto()
{
}

void Goto::setPoint(const Point &p)
{
	if(pTarget) pTarget = NULL;
	target = p;
}

void Goto::setPoint(Point *p)
{
	pTarget = p;
}

void Goto::setPoint(qreal x, qreal y)
{
	if(pTarget) pTarget = NULL;
	target.setX(x);
	target.setY(y);
}

void Goto::setLookAt(Point *p)
{
	lookAt = p;
}

void Goto::setOrientation(qreal o)
{
	lookAt = NULL;
	Steer::setOrientation(o);
}

void Goto::setOrientation(qreal dx, qreal dy)
{
	lookAt = NULL;
	Steer::setOrientation(dx, dy);
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

void Goto::step(Point *optionalPoint)
{
	Robot* robot = this->robot();
	//qreal targetTempX, targetTempY//unused
    qreal speedX, speedY, speedTemp, k;
	qreal targetX, targetY;
	if(optionalPoint) {
		targetX = optionalPoint->x();
		targetY = optionalPoint->y();
	} else if(pTarget) {
		targetX = pTarget->x();
		targetY = pTarget->y();
	} else {
		targetX = target.x();
		targetY = target.y();
	}

	//limitar dentro do campo
	qreal limit = robot->body().radius() + stage()->ball()->radius();
	qreal maxX = stage()->fieldLength() / 2 + limit;
	qreal minX = -maxX;
	qreal maxY = stage()->fieldWidth() / 2 + limit;
	qreal minY = -maxY;
	targetX = targetX < minX ? minX : targetX > maxX ? maxX : targetX;
	targetY = targetY < minY ? minY : targetY > maxY ? maxY : targetY;

	//limitar fora da area de defesa
	if(!allowDefenseArea) {
		if(isInDefenseArea(targetCopy(), robot, robot->team()->goal()))
		{
			Point corrected(awayFromDefenseArea(targetCopy(), robot, robot->team()->goal()));
			targetX = corrected.x();
			targetY = corrected.y();
			Line path(*robot, corrected);
			for(uint t = 0.0; t < AREADISC; t++) {
				Point p(path.pointAt(t / AREADISC));
				if(isInDefenseArea(p, robot, robot->team()->goal())) {
					Point corrected(awayFromDefenseArea(targetCopy(), robot, robot->team()->goal()));
					targetX = corrected.x();
					targetY = corrected.y();
					break;
				}
			}
		}
	}

	/*
	//Controle utilizado no m�xico 2012
	//controle speedX 
	controllerSpeedX.entrada = targetX; //deseja-se que a dist�ncia entre o alvo e o rob� seja igual a zero
	controllerSpeedX.realimentacao = robot->x();
	pidService(controllerSpeedX);
	speedX = controllerSpeedX.saida;
	//controle speedY
	controllerSpeedY.entrada = targetY; //deseja-se que a dist�ncia entre o alvo e o rob� seja igual a zero
	controllerSpeedY.realimentacao = robot->y();
	pidService(controllerSpeedY);
	speedY = controllerSpeedY.saida;
	//calculo speed linear
	speedTemp = sqrt(speedX*speedX + speedY*speedY);
	if(speedTemp > speed || ignoreBrake)
	{
		k = speed/speedTemp;
		speedX *= k;
		speedY *= k;
	}*/
	

	/*//Controle Explicito 1
	qreal errorX = (targetX - robot->x());
	qreal errorY = (targetY - robot->y());
	qreal cte1 = 1000;
	qreal cte2 = 300;
	qreal error = sqrt(errorX*errorX + errorY*errorY); 
	qreal speedAux = speed;
	
	if(error < cte1)
		speedAux = speed/3.;
	if(error > cte2)
		speedTemp = speedAux;
	else{
		speedTemp = (-2*pow(error/cte2,3) + 3*pow(error/cte2,2))*speedAux;
	}
	speedX = speedTemp*(errorX/error);
	speedY = speedTemp*(errorY/error);
	*/

	//Controle Explicito 2
	/*qreal errorX = (targetX - robot->x());
	qreal errorY = (targetY - robot->y());
	qreal t0 = 100;
	qreal t1 = 3000;
	qreal a = 5*t0;
	qreal b = speed;
	qreal error = sqrt(errorX*errorX + errorY*errorY); 
	//qreal speedAux = speed;//SA: Dead store and unused variable
	if(error > t1)
		speedTemp = speed;
	else if(error > t0)
		speedTemp = ((error-t0)/(t1-t0))*b + ((error-t1)/(t0-t1))*a;
	else{
		speedTemp = 5*error;
	}
	speedTemp*=0.7;
	speedX = speedTemp*(errorX/error);
	speedY = speedTemp*(errorY/error);*/

	//Controle Explicito 3
	qreal errorX = (targetX - robot->x());
	qreal errorY = (targetY - robot->y());
	qreal error = sqrt(errorX*errorX + errorY*errorY); 
	qreal g = 9.80665 * 1000;
	qreal mi = 0.4;
	qreal aMax = mi*g;
	qreal vMax = speed;
	qreal cte = 4*(aMax/(vMax*vMax));
	speedTemp = vMax * ( 1 - exp(-cte*error) );
	speedX = speedTemp*(errorX/error);
	speedY = speedTemp*(errorY/error);

	// adding plusSpeed
	//speedX += 2 * plusSpeed_.x();
	//speedY += 2 * plusSpeed_.y();

	if(lookAt) Steer::setOrientation(DEGTORAD(Line(*robot, *lookAt).angle()));
	Steer::setSpeeds(speedX, speedY);
	Steer::step();
}
