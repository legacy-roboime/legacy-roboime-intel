#include "ForceFieldMotion.h"
#include "Robot.h"
#include "Stage.h"
#include "Team.h"
#include "Ball.h"
#include <cmath>
#include "SteerToPoint.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

ForceFieldMotion::ForceFieldMotion(QObject* p, Robot* r, qreal x, qreal y, qreal o, qreal s, qreal re, qreal a)
	: Steer(p, r, r->x()-x, r->y()-y, o),
	targetX(x),
	targetY(y),
	speed(s),
	repulsion(re),
	attraction(a),
	ignoreBall(false),
	ignoreFriend(false),
	ignoreEnemy(false)
{}

void ForceFieldMotion::setIgnoreBall()
{
	ignoreBall = true;
}

void ForceFieldMotion::setNotIgnoreBall()
{
	ignoreBall = false;
}

void ForceFieldMotion::setIgnoreEnemy()
{
	ignoreEnemy = true;
}

void ForceFieldMotion::setNotIgnoreEnemy()
{
	ignoreEnemy = false;
}

void ForceFieldMotion::setIgnoreFriend()
{
	ignoreFriend = true;
}

void ForceFieldMotion::setNotIgnoreFriend()
{
	ignoreFriend = false;
}

void ForceFieldMotion::step()
{
	static qreal fx, fy, dx, dy, o, f, d;
	fx = 0.0;
	fy = 0.0;

	//target point attraction
	dx = targetX - robot()->x();
	dy = targetY - robot()->y();
	o = atan2(dy, dx);
	d = sqrt(dx * dx + dy * dy);
	f = attraction;// / d / d;
	fx += f * cos(o);
	fy += f * sin(o);

	//ball repulsion
	if(!ignoreBall) {
		dx = stage()->ball()->x() - robot()->x();
		dy = stage()->ball()->y() - robot()->y();
		d = sqrt(dx * dx + dy * dy) - stage()->ball()->radius() - robot()->body().radius();
		o = atan2(dy, dx);
		f = repulsion / d / d / 4;
		fx -= f * cos(o);
		fy -= f * sin(o);
	}

	//friend robots repulsion
	Team* friendTeam = robot()->team();
	if(friendTeam != 0 && !ignoreFriend) for(int i = 0; i < friendTeam->size(); i++) {
		Robot* r = friendTeam->at(i);
		if(r != robot()) {
			dx = r->x() - robot()->x();
			dy = r->y() - robot()->y();
			d = sqrt(dx * dx + dy * dy) - r->body().radius() - robot()->body().radius();
			o = atan2(dy, dx);
			f = repulsion / d / d;
			fx += f * sin(o);
			fy -= f * cos(o);
		}
	}

	//enemy robots repulsion
	Team* enemyTeam = robot()->enemyTeam();
	if(enemyTeam != 0 && !ignoreEnemy) for(int i = 0; i < enemyTeam->size(); i++) {
		Robot* r = enemyTeam->at(i);
		dx = r->x() - robot()->x();
		dy = r->y() - robot()->y();
		d = sqrt(dx * dx + dy * dy) - r->body().radius() - robot()->body().radius();
		o = atan2(dy, dx);
		f = repulsion / d / d;
		fx += f * sin(o);
		fy -= f * cos(o);
	}

	fx = targetX - robot()->x();
	fy = targetY - robot()->y();

	//normalization and stepping
	f = sqrt(fx * fx + fy * fy);
	fx /= f;
	fy /= f;
	//static SteerToBall stb(this, robot(), 0.0, 0.0);
	//stb.setSpeeds(speed * fx, speed * fy);
	//stb.step();

	Steer::setSpeeds(speed * fx, speed * fy);
	Steer::step();
}

void ForceFieldMotion::setAll(qreal x, qreal y, qreal s, qreal r, qreal a)
{
	setPoint(x,y);
	setSpeed(s);
	setRepulsion(r);
	setAttraction(a);
}

void ForceFieldMotion::setPoint(qreal _x, qreal _y)
{
	targetX = _x;
	targetY = _y;
}

void ForceFieldMotion::setSpeed(qreal s)
{
	speed = s;
}

void ForceFieldMotion::setRepulsion(qreal r)
{
	repulsion = r;
}

void ForceFieldMotion::setAttraction(qreal a)
{
	attraction = a;
}
