#include "Object.h"
#include <QVector>
#include <QList>

using namespace LibIntelligence;

Object::Object(qreal x, qreal y, qreal sx, qreal sy, qreal t, qreal o)
	: Point(x, y),
	speed_(sx, sy),
	theta_(t),
	omega_(o)
{}

Object::Object(const Object& object)
	: Point((Point) object),
	speed_(object.speed_),
	theta_(object.theta_),
	omega_(object.omega_)
{}

void Object::updatePosition(const Point &p)
{
	setX(p.x());
	setY(p.y());
}

void Object::updateSpeed(double time)
{
	double deltaTime = (time - this->timeOld_);
	if(deltaTime != 0)
	{
		speed_.setX((x() - posOld_.x())/deltaTime);
		speed_.setY((y() - posOld_.y())/deltaTime);

		if(abs(this->orientation() - thetaOld_) <  1){		//TODO: GAMBIARRA PARA SOLUCIONAR O PROBLEMA DE TRANSIÇÃO DE 2PI PARA 0 
			omega_ =  (this->orientation() - thetaOld_)/deltaTime;
		}

		posOld_.setX(x());
		posOld_.setY(y());
		thetaOld_ = this->orientation();
		timeOld_ = time;
	}
}

qreal Object::orientation() const
{
	return theta_;
}

void Object::setOrientation(qreal o)
{
	theta_ = o;
}

Vector Object::speed() const
{
	return speed_;
}

void Object::setSpeed(const Vector &s)
{
	speed_ = s;
}

void Object::setSpeed(const qreal &sx, const qreal &sy)
{
	speed_ = Vector(sx, sy);
}

qreal Object::angSpeed() const
{
	return omega_;
}

void Object::setAngSpeed(const qreal &as)
{
	omega_ = as;
}
