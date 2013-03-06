#include "Actuator.h"

using namespace LibIntelligence;

Actuator::Actuator(const Actuator& a)
	: Component(a.parent()),
	speed_(a.speed()) {}

Actuator::Actuator(QObject* p, qreal s)
	: Component(p),
	speed_(s) {}

void Actuator::setSpeed(qreal s)
{
	speed_ = s;
}

qreal Actuator::speed() const
{
	return speed_;
}
