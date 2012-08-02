#include "FollowAndCover.h"
#include "mathutils.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

FollowAndCover::FollowAndCover(QObject *parent, Robot *slave, Point *follow, Point *cover, qreal followDistance, qreal speed)
	: Goto(parent, slave, 0.0, 0.0, 0.0, speed),
	follow(follow),
	cover(cover),
	distance(followDistance)
{}

void FollowAndCover::setFollow(Point *p)
{
	follow = p;
}

void FollowAndCover::setCover(Point *p)
{
	cover = p;
}

void FollowAndCover::setDistance(qreal d)
{
	distance = d;
}

void FollowAndCover::setSpeed(qreal s)
{
	Goto::setSpeed(s);
}

void FollowAndCover::step()
{
	//connect the dots :)
	Line base(*follow, *cover);

	//when we set the length P1 remains constant, P2 moves so the angle is preserved
	base.setLength(distance);
	Goto::setPoint(base.p2());

	//we may also want to orient torwards the followed
	//keep in mind we use radians and qt uses degrees
	//TODO: use degrees too, it's easy
	Goto::setOrientation(DEGTORAD(180 + base.angle()));

	//that's all folks!
	Goto::step();
}