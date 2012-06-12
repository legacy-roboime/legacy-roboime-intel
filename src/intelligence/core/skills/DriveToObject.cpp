#include "DriveToObject.h"
#include <QLineF>
#include "Robot.h"
#include "Sampler.h"

#define CART	70 //DISTANCIA DO CORTE DA CARRENAGEM

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveToObject::DriveToObject(QObject* parent, Robot* slave, const Object* object, qreal radiusObj, const Object* refLookPoint, qreal maxErrorD, qreal maxErrorA, qreal speed, bool deterministic, qreal maxAngVar)
	: DriveTo(parent, slave, maxErrorD, maxErrorA, 0, QPointF(0,0), CART/*slave->body().radius()*/ + radiusObj, 0, speed),
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
	QLineF target = QLineF(object->x(), object->y(), lookPoint->x(), lookPoint->y());

	if(!deterministic_){
		qreal angle = maxAngVar_; //graus
		if(Sampler::randFloat()>0.5)
			target.setAngle(target.angle() + Sampler::randFloat()*angle);
		else
			target.setAngle(target.angle() - Sampler::randFloat()*angle);
	}
	bPoint.setX(object->x());
	bPoint.setY(object->y());

	//QLineF ttt = QLineF(0,0,1,1); //ESTRANHO: SAIDA 315
	//qreal anggg = ttt.angle(); //ESTRANHO: SAIDA 315

	bAngle = M_PI - target.angle() * M_PI / 180.; //angulo na classe QLineF eh invertido (positivo no sentido horario, estranho pq na documentacao tem o contrario), alem disso eh em graus
	tAngle = M_PI + bAngle;

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
//	QLineF target = QLineF(lookPoint->x(), lookPoint->y(), object->x(), object->y());
//	bPoint.setX(object->x());
//	bPoint.setY(object->y());
//	bAngle = -target.angle() * M_PI / 180.; //angulo na classe QLineF eh invertido (positivo no sentido horario, estranho pq na documentacao tem o contrario), alem disso eh em graus
//	tAngle = M_PI + bAngle;
//	//if(tAngle > 2*M_PI)
//	//	tAngle -= 2*M_PI;
//}

bool DriveToObject::busy()
{
	//QLineF target = QLineF(lookPoint->x(), lookPoint->y(), object->x(), object->y()); //posicao oposta ao lookPoint
	//bPoint.setX(object->x());
	//bPoint.setY(object->y());
	//bAngle = -target.angle() * M_PI / 180.; //angulo na classe QLineF eh invertido (positivo no sentido horario, estranho pq na documentacao tem o contrario), alem disso eh em graus
	//tAngle = M_PI + bAngle;
	//if(tAngle > 2*M_PI)
	//	tAngle -= 2*M_PI;

	return DriveTo::busy();
}