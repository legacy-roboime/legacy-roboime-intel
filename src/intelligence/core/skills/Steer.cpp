#include "Steer.h"
#include "Robot.h"
#include "PID.h"
#include "mathutils.h"

#define RATE	6.2//2.50
#define M_2PI	6.2831853071795865
#ifndef M_PI
#define M_PI	3.1415926535897932
#endif
#define __q(x)	((x) > M_PI ? (x) - M_2PI : -(x) > M_PI ? (x) + M_2PI : (x))
//qreal __n(qreal ang) {return ang > M_PI ? __n(ang - M_2PI) : -ang > M_PI ? __n(ang + M_2PI) : ang;}
#define CART1 1.8

using namespace LibIntelligence;
using namespace Skills;

Steer::Steer(QObject* p, Robot* r, Vector s, Point *l)
	: Move(p, r, s.x(), s.y()),
	lookPoint(l),
	rate(RATE),
	controller(CART1, 0.0, 0.0, 12.0, 2.0)//valores carteados CART1
{}

Steer::Steer(QObject* p, Robot* r, qreal sx, qreal sy, qreal o)
	: Move(p, r, sx, sy),
	lookPoint(NULL),
	orientation(o),
	rate(RATE),
	controller(CART1, 0.0, 0.0, 12.0, 2.0)//valores carteados CART1
{}

Steer::Steer(QObject* p, Robot* r, qreal sx, qreal sy, qreal dx, qreal dy)
	: Move(p, r, sx, sy),
	lookPoint(NULL),
	orientation(atan2(dy,dx)),
	rate(RATE),
	controller(CART1, 0.0, 0.0, 12.0, 2.0)//valores carteados CART1
{}

void Steer::setRate(qreal r)
{
	rate = r;
}

void Steer::setLookPoint(Point *p)
{
	lookPoint = p;
}

void Steer::step()
{
	qreal angleF = RADTODEG(lookPoint ? DEGTORAD(Line(*robot(), *lookPoint).angle()) : orientation);
	int angleI = ( (int)angleF ) % 360;
	qreal frac = angleF - (int)angleF; //manter precisão do double
	angleF = DEGTORAD(angleI + frac);

	/*controller.entrada = __q( angleF - robot()->orientation() );
	controller.realimentacao = 0.0;
	pidService(controller);
	Move::setSpeedAngular(controller.saida);*/
	
	qreal error = __q( angleF - robot()->orientation() );
	qreal g = 9.80665 * 1000;
	qreal mi = 0.4;
	qreal aMax = 8*mi*g/(robot()->body().radius());
	qreal vMax = 250; //Valor carteado, mas na forma analitica deveria ser (velocidade linear maxima)/(raio do robo), velocidade linear maxima eh a speed do Goto
	qreal k = 4*(aMax/(vMax*vMax));
	Move::setSpeedAngular( vMax * ( 1 - exp(-k*error) ) );

	Move::step();
}

void Steer::setAll(qreal sx, qreal sy, qreal o)
{
	Move::setSpeeds(sx, sy);
	setOrientation(o);
}

void Steer::setPIDk(double kp, double ki, double kd)
{
	controller.Kp = kp;
	controller.Ki = ki;
	controller.Kd = kd;
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
