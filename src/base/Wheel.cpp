#include "Wheel.h"

using namespace LibIntelligence;

Wheel::Wheel(const Wheel& w)
	: Actuator(w.parent(), w.speed()),
	angle_(w.angle()),
	radius_(w.radius()),
	distance_(w.distance()) {}

Wheel::Wheel(const Wheel* w)
	: Actuator(w->parent(), w->speed()),
	angle_(w->angle()),
	radius_(w->radius()),
	distance_(w->distance()) {}

Wheel::Wheel(QObject* p, qreal a, qreal r, qreal d, qreal s)
	: Actuator(p, s),
	angle_(a),
	radius_(r),
	distance_(d) {}

void Wheel::setAngle(qreal a)
{
	angle_ = a;
}

qreal Wheel::angle() const
{
	return angle_;
}
void Wheel::setRadius(qreal r)
{
	radius_ = r;
}

qreal Wheel::radius() const
{
	return radius_;
}
void Wheel::setDistance(qreal d)
{
	distance_ = d;
}

qreal Wheel::distance() const
{
	return distance_;
}
