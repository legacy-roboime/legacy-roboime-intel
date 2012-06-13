#include "Steer.h"
#include "Robot.h"
#include "PID.h"
#include "mathutils.h"

using namespace LibIntelligence;
using namespace Skills;

Steer::Steer(QObject* p, Robot* r, qreal sx, qreal sy, qreal o)
	: Move(p, r, sx, sy),
	rate(RATE),
	orientation(o),
	controller(1.8, 0.0, 0.0, 12.0, 2.0)//valores carteados 1.8
{}

Steer::Steer(QObject* p, Robot* r, qreal sx, qreal sy, qreal dx, qreal dy)
	: Move(p, r, sx, sy),
	rate(RATE),
	orientation(atan2(dy,dx)),
	controller(1.8, 0.0, 0.0, 12.0, 2.0)//valores carteados 1.8
{}

void Steer::setRate(qreal r)
{
	rate = r;
}

void Steer::step()
{
	qreal omega;
	//P Control
	//qreal aThreshold = M_PI;
	//qreal errorP = -2.*pow(omega/aThreshold,3)+3*pow(omega/aThreshold,2);
	controller.entrada = __q(orientation - robot()->orientation());
	controller.realimentacao = 0.0;
	pidService(controller);
	Move::setSpeedAngular(controller.saida);
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
