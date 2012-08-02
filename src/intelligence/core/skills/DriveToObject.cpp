#include "DriveToObject.h"
#include "Robot.h"
#include "Sampler.h"
#include "geomutils.h"
#include "mathutils.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveToObject::DriveToObject(QObject* parent, Robot* slave, const Object* object, qreal radiusObj, const Object* refLookPoint, qreal maxErrorD, qreal maxErrorA, qreal speed, bool deterministic, qreal maxAngVar)
	: DriveTo(parent, slave, maxErrorD, maxErrorA, 0, Point(0,0), slave->body().cut() + radiusObj, 0, speed),
	refLookPoint_(refLookPoint),
	lookPoint(new Object()),
	maxAngVar_(maxAngVar)
{
	this->object = object;
	deterministic_ = deterministic;
}

DriveToObject::~DriveToObject(void)
{
	delete lookPoint;
}

//Robo se posiciona em posicao oposta ao lookPoint em relação o object ficando alinha lookPoint, object e robo (nessa ordem) 
void DriveToObject::step()
{
	*lookPoint = *refLookPoint_;
	Line target = Line(object->x(), object->y(), lookPoint->x(), lookPoint->y());

	if(!deterministic_){
		qreal angle = maxAngVar_; //graus
		if(Sampler::randFloat()>0.5)
			target.setAngle(target.angle() + Sampler::randFloat()*angle);
		else
			target.setAngle(target.angle() - Sampler::randFloat()*angle);
	}
	bPoint.setX(object->x());
	bPoint.setY(object->y());

	bAngle = M_PI + DEGTORAD(target.angle());
	tAngle = bAngle - M_PI;

	DriveTo::step();
}

const Object* DriveToObject::getRefLookPoint() const
{
	return refLookPoint_;
}

void DriveToObject::setRefLookPoint(const Object* refLookPoint)
{
	refLookPoint_ = refLookPoint;
}

const Object* DriveToObject::getLookPoint() const
{
	return lookPoint;
}

//void DriveToObject::setLookPoint(Object* lookPoint)
//{
//	this->lookPoint = lookPoint;
//
//	Line target = Line(lookPoint->x(), lookPoint->y(), object->x(), object->y());
//	bPoint.setX(object->x());
//	bPoint.setY(object->y());
//	bAngle = target.angle() * M_PI / 180.;
//	tAngle = M_PI + bAngle;
//	//if(tAngle > 2*M_PI)
//	//	tAngle -= 2*M_PI;
//}

bool DriveToObject::busy()
{
	//Line target = Line(lookPoint->x(), lookPoint->y(), object->x(), object->y()); //posicao oposta ao lookPoint
	//bPoint.setX(object->x());
	//bPoint.setY(object->y());
	//bAngle = target.angle() * M_PI / 180.; 
	//tAngle = M_PI + bAngle;
	//if(tAngle > 2*M_PI)
	//	tAngle -= 2*M_PI;

	return DriveTo::busy();
}