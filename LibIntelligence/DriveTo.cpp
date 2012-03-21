#include "DriveTo.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include <QLineF>

#define M_PI	3.1415926535897932

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveTo::DriveTo(QObject* parent, Robot* slave, qreal angle, QPointF point, qreal threshold, qreal speed)
	: Goto(parent, slave),
	wait(new Wait(parent, slave))
{
	this->speed = speed;
	this->bAngle = angle;
	this->threshold = threshold;
	this->bPoint = point;
	ignoreBall = false;
	ignoreEnemy = false;
	ignoreFriend = false;
	ignoreBrake = false;
}


DriveTo::~DriveTo(void)
{
	delete wait;
}

//Goto em coordenadas polares
void DriveTo::step()
{
	Stage* stage = this->stage();
	Robot* robot = this->robot();

	QLineF target = QLineF(bPoint.x(), bPoint.y(), bPoint.x() + cos(bAngle), bPoint.y() + sin(bAngle));
	target.setLength(threshold);
	QPointF p = target.p2();
	qreal x = p.x();
	qreal y = p.y();

	tAngle = M_PI + bAngle;
	tPoint = p;

	if(tAngle > 2*M_PI)
		tAngle -= 2*M_PI;

	Goto::setPoint(x, y);
	Goto::setSpeed(speed);
	Goto::setOrientation(tAngle);
	Goto::step();
}

bool DriveTo::busy(){
	Stage* stage = this->stage();
	Robot* robot = this->robot();
	
	qreal x = robot->x();
	qreal y = robot->y();
	qreal errorX = tPoint.x() - x;
	qreal errorY = tPoint.y() - y;
	qreal errorD = sqrt(errorX*errorX + errorY*errorY);

	qreal orientation = robot->orientation();
	qreal errorA = abs(tAngle - orientation);

	//printf("%f\n", errorA * 180. / M_PI);

	if(errorD < 50. && errorA < M_PI/90.)
		return false;
	else
		return true;
}