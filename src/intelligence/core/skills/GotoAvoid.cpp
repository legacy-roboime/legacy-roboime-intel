#include "GotoAvoid.h"
#include "mathutils.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

#define LIMIT 5
#define KP 2.0
#define KI 0.0
#define KD 0.0
#define LIMERR 2.0

GotoAvoid::GotoAvoid(QObject *parent, Robot *slave, Point *target, Point *avoid, qreal speed)
	: Goto(parent, slave, target, 0.0, speed),
	circlePID(KP, KI, KD, DBL_MAX, LIMERR),
	avoid(avoid),
	tangPoint(new Point())
{}

GotoAvoid::~GotoAvoid()
{
	delete tangPoint;
}

void GotoAvoid::setTarget(Point *p)
{
	Goto::setPoint(p);
}

void GotoAvoid::setAvoid(Point *p)
{
	avoid = p;
}

void GotoAvoid::setSpeed(qreal s)
{
	Goto::setSpeed(s);
}

void GotoAvoid::step()
{
	//shortcuts
	Robot &robot = *this->robot();

	//robot-target-avoid angle
	Line robot_target(robot, pTarget ? *pTarget : target);
	Line avoid_target(*avoid, pTarget ? *pTarget : target);
	qreal rta_angle = robot_target.angleTo(avoid_target);

	Steer::setOrientation(DEGTORAD(avoid_target.angle()));
	if(rta_angle >= 90) {
		// Then we can go straight to the point
		Goto::setNotIgnoreBrake();
		Goto::step();

	} else if(fabs(robot_target.length() - avoid_target.length()) <= LIMIT) {
		// If close enough of the tangent point orbit around it with control
		circlePID.entrada = avoid_target.angle();
		circlePID.realimentacao = robot_target.angle();
		pidService(circlePID);
		Move::setSpeedAngular(DEGTORAD(circlePID.saida));//the PID will give us the angular speed
		Line speedVec(robot_target.normalVector());//speed vector is tangent to the circle
		speedVec.setLength(circlePID.saida * robot_target.length());//speed = omega * radius
		Move::setSpeeds(speedVec.dx(), speedVec.dy());
		Move::step();

	} else {
		// Otherwise go to the tangent point
		//this will only work if we're outside
		//FIXME: this breaks loose when we're inside the circle
		// find out the deviation angle to the tangent point from the avoid point
		Line robot_avoid(robot, *avoid);
		qreal sinTheta(avoid_target.length() / robot_avoid.length());
		qreal theta(RADTODEG(asinf(sinTheta)));
		qreal dist(sqrt(SQ(robot_avoid.length()) - SQ(avoid_target.length())));
		Point tangPoint1(Line::fromPolar(dist, robot_avoid.angle() + theta).translated(robot).p2());
		Point tangPoint2(Line::fromPolar(dist, robot_avoid.angle() - theta).translated(robot).p2());
		*tangPoint = Line(tangPoint1, targetCopy()).length() < Line(tangPoint2, targetCopy()).length() ? tangPoint1 : tangPoint2;
		Goto::step(tangPoint);
	}
}