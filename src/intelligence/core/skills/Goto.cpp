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

Point intersceptLineCircle(Line path, const Point &center, const qreal &radius)
{
	//get the normal line starting at the center of the circle
	Line normal(Line::fromPolar(1, path.angle() + 90).translated(center));
	//make it a segment to the intersection with the path
	Point temp;
	normal.intersect(path, &temp);
	normal.setP2(temp);
	//the distance to the circle intersception
	//XXX: warning this will only work if path.p1() is outside and path.p2() is inside the circle
	qreal dist(sqrt(SQ(radius) - SQ(normal.length())));
	//the magic, it's hard to explain:
	path.setP2(temp);
	path.setLength(path.length() - dist);
	//believe me this is right
	return path.p2();
}

bool isInDefenseArea(const Line &path, const Robot *r, const Point *g, Point *corrected)
{
	Stage *s(r->stage());
	qreal halfStretch(s->defenseStretch() / 2);
	qreal radius(r->body().radius() + s->defenseRadius());
	// this is the stretch line over the goal
	Line base(g->x(), g->y() - halfStretch, g->x(), g->y() + halfStretch);

	//inside the rectangle
	if(path.p2().y() >= base.p1().y() && path.p2().y() <= base.p2().y() && fabs(path.p2().x() - g->x()) < radius) {
		//the defense line is the line which must not be crossed
		Line defenseLine(base.translated(g->x() > 0 ? -radius : radius, 0));
		//find out where we should stop on our way to that line
		defenseLine.intersect(path, corrected);
		return true;

	} else if(Line(path.p2(), base.p1()).length() < radius) {
		//this is the line which describes the movement we're doing
		*corrected = intersceptLineCircle(path, base.p1(), radius);
		return true;

	} else if(Line(path.p2(), base.p2()).length() < radius) {
		//this is the line which describes the movement we're doing
		*corrected = intersceptLineCircle(path, base.p2(), radius);
		return true;

	} else return false;
}

Goto::Goto(QObject* p, Robot* r, Point *t, qreal o, qreal s, bool a)
	: Steer(p, r, 0, 0, o),
	ignoreBrake(false),
	managedPoint(false),
	lookAt(NULL),
	allowDefenseArea(a),
	target(t),
	speed(s),
	old(0, 0),
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
	old(0, 0),
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
	/*if(!allowDefenseArea) {
		Point corrected;
		if(isInDefenseArea(Line(*target, old), robot(), robot()->team()->goal(), &corrected))
		{
			targetX = corrected.x();
			targetY = corrected.y();
		}
	}*/

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

	//save this iteration
	old.setX(targetX);
	old.setY(targetY);
}