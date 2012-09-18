#include "GotoAvoid.h"
#include "mathutils.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

#define LIMIT1 25
#define LIMIT2 25
#define LIMIT3 25
#define KP 2.0
#define KI 0.0
#define KD 0.0
#define LIMERR 2.0

GotoAvoid::GotoAvoid(QObject *parent, Robot *slave, Point *target, Point *avoid, qreal radiusAvoid, qreal speed)
	: Goto(parent, slave, target, 0.0, speed),
	avoid(avoid),
	tangPoint(new Point()),
	radiusAvoid(radiusAvoid)
{}

GotoAvoid::~GotoAvoid()
{
	delete tangPoint;
}

void GotoAvoid::setTarget(Point *p)
{
	Goto::setPoint(p);
}

void GotoAvoid::setTarget(qreal x, qreal y)
{
	Goto::setPoint(x, y);
}

Point* GotoAvoid::getAvoid()
{
	return avoid;
}

qreal GotoAvoid::getRadiusAvoid()
{
	return radiusAvoid;
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
	//TODO: implementar para um ponto generico, nao obrigatoriamente para um ponto proximo da bola.

	//shortcuts
	Robot* robot = this->robot();

	//robot-target-avoid angle
	Line robot_target(*robot, pTarget ? *pTarget : target);
	Line avoid_target(*avoid, pTarget ? *pTarget : target);
	Line avoid_robot(*avoid, *robot);
	Line target_avoid(pTarget ? *pTarget : target, *avoid);
	Line robot_avoid(*robot, *avoid);
	qreal rta_angle = robot_target.angleTo(avoid_target);

	Steer::setOrientation(DEGTORAD(target_avoid.angle()));
	if(rta_angle >= 90 && rta_angle <= 270) {
		// Then we can go straight to the point
		Goto::setNotIgnoreBrake();
		Goto::step();
	} else if(!(robot_avoid.length() > radiusAvoid + 500)){ 
		int dec = 20;
		int i;
		qreal delta = avoid_target.angleTo(avoid_robot);
		if(delta>10 && delta<350){ //margem de estabilidade
			if(delta<180)
				avoid_target.setAngle(avoid_robot.angle() - dec);
			else
				avoid_target.setAngle(avoid_robot.angle() + dec);
		}
		Point p = avoid_target.p2();
		Goto::setPoint(p.x(), p.y());
		Goto::step();
	}
	else { 
		//Go to the tangent point
		qreal sinTheta(avoid_target.length() / robot_avoid.length());
		qreal theta(RADTODEG(asinf(sinTheta)));
		qreal dist(sqrt(SQ(robot_avoid.length()) - SQ(avoid_target.length())));
		Point tangPoint1(Line::fromPolar(dist, robot_avoid.angle() + theta).translated(*robot).p2());
		Point tangPoint2(Line::fromPolar(dist, robot_avoid.angle() - theta).translated(*robot).p2());
		*tangPoint = Line(tangPoint1, targetCopy()).length() < Line(tangPoint2, targetCopy()).length() ? tangPoint1 : tangPoint2;
		Goto::step(tangPoint);
	}
}

bool GotoAvoid::busy()
{
	return Goto::busy();
}