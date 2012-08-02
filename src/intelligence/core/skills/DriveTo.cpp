#include "DriveTo.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "geomutils.h"
#include "mathutils.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveTo::DriveTo(QObject* parent, Robot* slave, qreal maxErrorD, qreal maxErrorA, qreal bAngle, Point point, qreal threshold, qreal tAngle, qreal speed)
	: Goto(parent, slave, (Point*)NULL, (Point*)NULL, speed)
{
	this->maxErrorA = maxErrorA;
	this->maxErrorD = maxErrorD;
	this->speed = speed;
	this->bAngle = bAngle;
	this->threshold = threshold;
	this->bPoint = point;
	this->tAngle = tAngle;
	ignoreBrake = false;
	Skill::deterministic_ = true;
}

//Goto em coordenadas polares
void DriveTo::step()
{
	Line target = Line(bPoint.x(), bPoint.y(), bPoint.x() + cos(bAngle), bPoint.y() +sin(bAngle));
	target.setLength(threshold);
	Point p = target.p2();
	qreal x = p.x();
	qreal y = p.y();
	tPoint = p;

	Goto::setPoint(x, y);
	Goto::setSpeed(speed);
	Goto::setOrientation(tAngle);
	Goto::step();
}

bool DriveTo::busy()
{
	Robot* robot = this->robot();
	
	qreal x = robot->x();
	qreal y = robot->y();
	qreal errorX = tPoint.x() - x;
	qreal errorY = tPoint.y() - y;
	qreal errorD = sqrt(errorX*errorX + errorY*errorY);

	qreal orientation = robot->orientation();

	qreal errorA = abs(tAngle - orientation); //tAngle e orientation entre 0 e 2PI
	if( errorA > M_PI )
		errorA = 2 * M_PI - errorA;

	//printf("%f %f %f\n", errorA * 180. / M_PI, tAngle, orientation);
	//cout << maxErrorD << " " << maxErrorA << endl;

	if(errorD < maxErrorD && errorA < maxErrorA)
		return false;
	else
		return true;
}
