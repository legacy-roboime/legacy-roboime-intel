#define _USE_MATH_DEFINES
#include <cmath>
#include "KickTo.h"
#include "GetBall.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "geomutils.h"
#include "mathutils.h"

using std::exp;
using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

KickTo::KickTo(QObject* p, Robot* r, qreal a)
	: Steer(p, r),
	angle(a),
	controllerDist(5,1,0,3000,1000)
{
}

void KickTo::setAngle(qreal _angle)
{
	angle = _angle;
}

qreal KickTo::getAngle()
{
	return angle;
}

void KickTo::step()
{
	Stage *stage=this->stage();
	Ball *ball=stage->ball();
	Robot *robot=this->robot();
	
	qreal g = 9.80665 * 1000;
	qreal mi = 0.4;
	qreal aMax = mi*g;
	qreal vMax = 1600;
	qreal cte = 4*(aMax/(vMax*vMax));

	qreal distToBall=Line(*robot,*ball).length();

	Line line(*ball,point_);
	qreal h=line.distanceTo(*robot);

	Line normal=line.normalVector().translated(*robot - line.p1());
	Point closestPoint;
	normal.intersect(line,&closestPoint);

	Point directionToBall(*ball-closestPoint);
	qreal d=Line(directionToBall,Point(0,0)).length();

	if(aproachSpeed<1){
		aproachSpeed=d*vMax/(2*h);
		aproachSpeed/=3;
		if(aproachSpeed>800 && d<300) aproachSpeed=800;
	}

	Point direction=closestPoint-*robot;
	direction/=Line(direction,Point(0,0)).length();
	direction*=vMax*( 1 - exp(-cte*h/2) );
		
	directionToBall/=d;
	directionToBall*=aproachSpeed;
	direction+=directionToBall;

	direction+=Point(ball->speed().x(), ball->speed().y());

	Steer::setSpeed(Vector(direction));
	Steer::setOrientation(Line(*robot,point_).angle()/180.0*M_PI);
	Steer::step();

	if(distToBall < (robot->body().radius()  + 2 * ball->radius())) {
		robot->kick();		
	}

}
