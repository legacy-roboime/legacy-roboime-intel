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
#define AREADISC 20

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
	ignoreBrake(false),
	managedPoint(false),
	lookAt(NULL),
	allowDefenseArea(a),
	target(t),
	speed(s),
	old(0.0, 0.0),
	controllerSpeedX(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, MAXCSPEED, LIMERR)//valores carteados
{}

Goto::Goto(QObject* p, Robot* r, Point *t, Point *l, qreal s, bool a)
	: Steer(p, r, 0, 0, 0),
	ignoreBrake(false),
	managedPoint(false),
	lookAt(l),
	allowDefenseArea(a),
	target(t),
	speed(s),
	old(0.0, 0.0),
	controllerSpeedX(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, MAXCSPEED, LIMERR)//valores carteados
{}

Goto::Goto(QObject* p, Robot* r, qreal x, qreal y, qreal o, qreal s, bool a)
	: Steer(p, r, 0, 0, o),
	ignoreBrake(false), 
	managedPoint(true),
	lookAt(NULL),
	allowDefenseArea(a), 
	target(new Point(0, 0)),
	speed(s),
	old(0.0, 0.0),
	controllerSpeedX(2, 0, 0.0, MAXCSPEED, LIMERR),//valores carteados
	controllerSpeedY(2, 0, 0.0, MAXCSPEED, LIMERR)//valores carteados
{}

Goto::~Goto()
{
	if(managedPoint)
		delete target;
}

void Goto::setManagedPoint()
{
	managedPoint = true;
}

void Goto::setNotManagedPoint()
{
	managedPoint = false;
}

void Goto::setPoint(const Point &p)
{
	if(!managedPoint) {
		target = new Point(p);
		managedPoint = true;
	} else {
		//*target = p;
		target->setX(p.x());
		target->setY(p.y());
	}
}

void Goto::setPoint(Point *p)
{
	if(managedPoint) delete target;
	target = p;
}

void Goto::setPoint(qreal x, qreal y)
{
	setPoint(Point(x, y));
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
	//TODO: valores objetivos devem ser alterados para valor nao deterministico (soma um float a speedx speedy e speedang)

	qreal targetTempX, targetTempY, speedX, speedY, speedTemp, k;
	qreal targetX, targetY;
	if(optionalPoint) {
		targetX = optionalPoint->x();
		targetY = optionalPoint->y();
	} else {
		targetX = target->x();
		targetY = target->y();
	}
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
		if(isInDefenseArea(*target, robot(), robot()->team()->goal()))
		{
			Point corrected(awayFromDefenseArea(*target, robot(), robot()->team()->goal()));
			targetX = corrected.x();
			targetY = corrected.y();
			Line path(*robot(), corrected);
			for(uint t = 0.0; t < AREADISC; t++) {
				Point p(path.pointAt(t / AREADISC));
				if(isInDefenseArea(p, robot(), robot()->team()->goal())) {
					Point corrected(awayFromDefenseArea(*target, robot(), robot()->team()->goal()));
					targetX = corrected.x();
					targetY = corrected.y();
					break;
				}
			}
		}
	}

	//controle speedX
	controllerSpeedX.entrada = targetX; //deseja-se que a distância entre o alvo e o robô seja igual a zero
	controllerSpeedX.realimentacao = robot()->x();
	pidService(controllerSpeedX);
	speedX = controllerSpeedX.saida;

	//controle speedY
	controllerSpeedY.entrada = targetY; //deseja-se que a distância entre o alvo e o robô seja igual a zero
	controllerSpeedY.realimentacao = robot()->y();
	pidService(controllerSpeedY);
	speedY = controllerSpeedY.saida;
	qreal a = Line(0, 0, 1, 1).angle();

	//calculo speed linear
	speedTemp = sqrt(speedX*speedX + speedY*speedY);
	if(speedTemp > speed || ignoreBrake)
	{
		k = speed/speedTemp;
		speedX *= k;
		speedY *= k;
	}

	if(lookAt) Steer::setOrientation(DEGTORAD(Line(*robot(), *lookAt).angle()));
	Steer::setSpeeds(speedX, speedY);
	Steer::step();
}