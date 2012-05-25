#include "Steer.h"
#include "Robot.h"
#include "PID.h"
#include <cmath>

#define RATE	6.2//2.50
#define M_2PI	6.2831853071795865
#define M_PI	3.1415926535897932
#define __q(x)	((x) > M_PI ? (x) - M_2PI : -(x) > M_PI ? (x) + M_2PI : (x))
//qreal __n(qreal ang) {return ang > M_PI ? __n(ang - M_2PI) : -ang > M_PI ? __n(ang + M_2PI) : ang;}

using namespace LibIntelligence;
using namespace Skills;

Steer::Steer(QObject* p, Robot* r, qreal sx, qreal sy, qreal o)
	: Move(p, r, sx, sy),
	rate(RATE),
	orientation(o) {}

Steer::Steer(QObject* p, Robot* r, qreal sx, qreal sy, qreal dx, qreal dy)
	: Move(p, r, sx, sy),
	rate(RATE),
	orientation(atan2(dy,dx)) {}

void Steer::setRate(qreal r)
{
	rate = r;
}

void Steer::step()
{
	static qreal omega;
	//P Control
	//qreal aThreshold = M_PI;
	//qreal errorP = -2.*pow(omega/aThreshold,3)+3*pow(omega/aThreshold,2);
	static CONTROLLER_S controlller(/*4.0*/1.8, 0.0, 0.0, 12.0, 2.0);//valores carteados
	controlller.entrada = __q(orientation - robot()->orientation());
	controlller.realimentacao = 0.0;
	pidService(controlller);
	Move::setSpeedAngular(controlller.saida);
	Move::step();
}

void Steer::setAll(qreal sx, qreal sy, qreal o)
{
	Move::setSpeeds(sx, sy);
	setOrientation(o);
}

void Steer::setAll(qreal sx, qreal sy, qreal dx, qreal dy)
{
	Move::setSpeeds(sx, sy);
	setOrientation(dx, dy);
}

void Steer::setOrientation(qreal o)
{
	orientation = o;
}

void Steer::setOrientation(qreal dx, qreal dy)
{
	orientation = atan2(dy,dx);
}
