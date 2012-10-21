#include "Object.h"
#include <QVector>
#include <QList>
#include <cstdio>
#include "Robot.h"
#include "mathutils.h"

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
	lo = 0;
	for (int i = 0; i < 4; ++i) {
		uo[i] = 0;
		vo[i] = 0;
	}
}

Object::Object(const Object& object)
	: Point((Point) object),
	speed_(object.speed_),
	theta_(object.theta_),
    omega_(object.omega_),
    timeOld_(object.timeOld_),
    thetaOld_(object.thetaOld_),
    posOld_(object.posOld_),
	lo(object.lo)
	//useFilter_(object.useFilter_)
{
	gain = 6.0;
	coef[0] = 3.0;
	coef[1] = 0.0;
	coef[2] = -1.0 / 3.0;
	coef[3] = 0.0;
	memcpy(ux,object.ux,4*sizeof(float));
	memcpy(vx,object.vx,4*sizeof(float));
	memcpy(uy,object.uy,4*sizeof(float));
	memcpy(vy,object.vy,4*sizeof(float));
	memcpy(uo,object.uo,4*sizeof(float));
	memcpy(vo,object.vo,4*sizeof(float));
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

		if(abs((long)(this->orientation() - thetaOld_)) <  1){//FIXME: GAMBIARRA PARA SOLUCIONAR O PROBLEMA DE TRANSIÇÃO DE 2PI PARA 0 
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

void Object::setOrientationWithFilter(qreal o)
{
	//XXX: TEST
	//using namespace std;
	//static FILE* f = fopen("teste.csv", "w");
	//bool t = ((Robot*)this)->id() == 2;

	float d = o - lo;
	// Normalize d to stay between -180 and 180 degrees to add correctly
	while(d >= M_PI) d -= M_2PI;
	while(d < -M_PI) d += M_2PI;

	// orientation
	uo[0] = uo[1];
	uo[1] = uo[2];
	uo[2] = uo[3];
	uo[3] = d / gain;
	vo[0] = vo[1];
	vo[1] = vo[2];
	vo[2] = vo[3];
	vo[3] = (uo[0] + uo[3]) + coef[0] * (uo[1] + uo[2]) + (coef[1] * vo[0]) + (coef[2] * vo[1]) + coef[3] * vo[2];
	
	theta_ = theta_ + vo[3];
	while(theta_ >= M_PI) theta_ -= M_2PI;
	while(theta_ < -M_PI) theta_ += M_2PI;
	
	lo = o;
	//if(t) fprintf(f, "%f;%f;\n", d, vo[3]);
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
