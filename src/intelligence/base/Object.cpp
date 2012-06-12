#include "Object.h"
#include <qvector.h>
#include <qlist.h>

using namespace LibIntelligence;

Object::Object(qreal x, qreal y, qreal z, qreal sx, qreal sy, qreal sz, qreal orien, qreal asz)
	: x_(x),
	y_(y),
	z_(z),
	orientation_(orien),
	speedX_(sx),
	speedY_(sy),
	speedZ_(sz),
	angSpeedZ_(asz),
	xOld(x),
	yOld(y),
	orientationOld(orien),
	timeOld(0)
{}

Object::Object(const Object& object)
{
	this->x_ = object.x();
	this->y_ = object.y();
	this->z_ = object.z();
	this->orientation_ = object.orientation();
	this->angSpeedZ_ = object.angSpeedZ();
	this->speedX_ = object.speedX();
	this->speedY_ = object.speedY();
	this->speedZ_ = object.speedZ();
}

Object::~Object()
{
}


void Object::setX(qreal x)
{
	x_ = x;
}

qreal Object::x() const
{
	return x_;
}

void Object::setY(qreal y)
{
	y_ = y;
}


qreal Object::y() const
{
	return y_;
}

void Object::setZ(qreal z)
{
	z_ = z;
}

qreal Object::z() const
{
	return z_;
}

qreal Object::orientation() const
{
	return orientation_;
}

void Object::setSpeedX(qreal sx)
{
	speedX_ = sx;
}

qreal Object::speedX() const
{
	return speedX_;
}

void Object::setSpeedY(qreal sy)
{
	speedY_ = sy;
}

qreal Object::speedY() const
{
	return speedY_;
}

void Object::setSpeedZ(qreal sz)
{
	speedZ_ = sz;
}

qreal Object::speedZ() const
{
	return speedZ_;
}

void Object::setAngSpeedZ(qreal asz)
{
	angSpeedZ_ = asz;
}

qreal Object::angSpeedZ() const
{
	return angSpeedZ_;
}

void Object::setXY(qreal x, qreal y)
{
	setX(x);
	setY(y);
}

void Object::setXYZ(qreal x, qreal y, qreal z)
{
	setX(x);
	setY(y);
	setZ(z);
}

void Object::setSpeedXY(qreal sx, qreal sy)
{
	setSpeedX(sx);
	setSpeedY(sy);
}

void Object::setSpeedXYZ(qreal sx, qreal sy, qreal sz)
{
	setSpeedX(sx);
	setSpeedY(sy);
	setSpeedZ(sz);
}

Object& Object::operator=(const Object& object)
{
	this->x_ = object.x();
	this->y_ = object.y();
	this->z_ = object.z();
	this->speedX_ = object.speedX();
	this->speedY_ = object.speedY();
	this->speedZ_ = object.speedZ();
	return *this;
}

void Object::setOrientation(qreal o)
{
	orientation_ = o;
}


void Object::updateSpeed(double time)
{
	double deltaTime = (time - this->timeOld);
	if(deltaTime != 0)
	{
		speedX_ = (this->x() - this->xOld)/deltaTime;
		speedY_ = (this->y() - this->yOld)/deltaTime;

		if(abs(this->orientation() - this->orientationOld) <  1){		//TODO: GAMBIARRA PARA SOLUCIONAR O PROBLEMA DE TRANSIÇÃO DE 2PI PARA 0 
			angSpeedZ_ =  (this->orientation() - this->orientationOld)/deltaTime;
		}

		this->xOld = this->x();
		this->yOld = this->y();
		this->orientationOld = this->orientation();
		this->timeOld = time;
	}
}

Object Object::distance(const Object* object2) const
{
	return Object(object2->x() - x_, object2->y() - y_, object2->z() - z_);
}

qreal Object::module() const
{
	return sqrt(x_ * x_ + y_ * y_);
}