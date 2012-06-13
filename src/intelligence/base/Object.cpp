#include "Object.h"
#include <qvector.h>
#include <qlist.h>

using namespace LibIntelligence;

Object::Object(qreal x, qreal y, qreal sx, qreal sy, qreal t, qreal o)
	: QPointF(x, y),
	speed_(sx, sy),
	theta_(t),
	omega_(o)
{}

Object::Object(const Object& object)
	: QPointF((QPointF) object),
	speed_(object.speed_),
	theta_(object.theta_),
	omega_(object.omega_)
{}

void Object::updatePosition(const QPointF &p)
{
	setX(p.x());
	setY(p.y());
}

qreal Object::orientation() const
{
	return theta_;
}

void Object::setOrientation(qreal o)
{
	theta_ = o;
}

QVector2D Object::speed() const
{
	return speed_;
}

void Object::setSpeed(const QVector2D &s)
{
	speed_ = s;
}

void Object::setSpeed(const qreal &sx, const qreal &sy)
{
	speed_ = QVector2D(sx, sy);
}

qreal Object::angSpeed() const
{
	return omega_;
}

void Object::setAngSpeed(const qreal &as)
{
	omega_ = as;
}
