#include "Object.h"
#include <QVector>
#include <QList>
#include <cstdio>

using namespace LibIntelligence;

Object::Object(qreal x, qreal y, qreal sx, qreal sy, qreal t, qreal o)
	: Point(x, y),
	speed_(sx, sy),
	theta_(t),
    omega_(o),
    timeOld_(0),
    thetaOld_(0),
    posOld_(0, 0)//,
	//useFilter_(false)
{
	//low pass filter stuff
	//NOTE: not initializing filter vectors
	// they will get overriden anyway
	gain = 6.0;
	coef[0] = 3.0;
	coef[1] = 0.0;
	coef[2] = -1.0 / 3.0;
	coef[3] = 0.0;
}

Object::Object(const Object& object)
	: Point((Point) object),
	speed_(object.speed_),
	theta_(object.theta_),
    omega_(object.omega_),
    timeOld_(object.timeOld_),
    thetaOld_(object.thetaOld_),
    posOld_(object.posOld_)//,
	//useFilter_(object.useFilter_)
{
	coef[0] = 3.0;
	coef[1] = 0.0;
	coef[2] = -1.0 / 3.0;
	coef[3] = 0.0;
}

void Object::updatePositionWithFilter(const Point &p)
{
	//XXX: TEST
	//using namespace std;
	//static FILE* f = fopen("teste.csv", "a");

	// x-coord
	ux[0] = ux[1];
	ux[1] = ux[2];
	ux[2] = ux[3];
	ux[3] = p.x() / gain;
	vx[0] = vx[1];
	vx[1] = vx[2];
	vx[2] = vx[3];
	vx[3] = (ux[0] + ux[3]) + coef[0] * (ux[1] + ux[2]) + (coef[1] * vx[0]) + (coef[2] * vx[1]) + coef[3] * vx[2];
	setX(vx[3]);

	// y-coord
	uy[0] = uy[1];
	uy[1] = uy[2];
	uy[2] = uy[3];
	uy[3] = p.y() / gain;
	vy[0] = vy[1];
	vy[1] = vy[2];
	vy[2] = vy[3];
	vy[3] = (uy[0] + uy[3]) + coef[0] * (uy[1] + uy[2]) + (coef[1] * vy[0]) + (coef[2] * vy[1]) + coef[3] * vy[2];
	setY(vy[3]);

	//TODO: identify object
	//fprintf(f, "%f;%f;%f;\n", bx, by, sx);
}

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

		if(abs((long)(this->orientation() - thetaOld_)) <  1){		//TODO: GAMBIARRA PARA SOLUCIONAR O PROBLEMA DE TRANSIÇÃO DE 2PI PARA 0 
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
