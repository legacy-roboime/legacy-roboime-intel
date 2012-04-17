#include "DriveTo.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include <QLineF>

#define M_PI	3.1415926535897932

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveTo::DriveTo(QObject* parent, Robot* slave, qreal angle, QPointF point, qreal threshold, qreal tAngle, qreal speed)
	: Goto(parent, slave)
{
	this->speed = speed;
	this->bAngle = angle;
	this->threshold = threshold;
	this->bPoint = point;
	this->tAngle = tAngle;
	ignoreBall = false;
	ignoreEnemy = false;
	ignoreFriend = false;
	ignoreBrake = false;
	Skill::deterministic_ = true;
}


DriveTo::~DriveTo(void)
{
}

//Goto em coordenadas polares
void DriveTo::step()
{
	QLineF target = QLineF(bPoint.x(), bPoint.y(), bPoint.x() + cos(bAngle), bPoint.y() + sin(bAngle));
	target.setLength(threshold);
	QPointF p = target.p2();
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

	if(errorD < 50. && errorA < 5 * M_PI/180.)
		return false;
	else
		return true;
}