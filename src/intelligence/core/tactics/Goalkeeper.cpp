#include "Goalkeeper.h"
#include "Steer.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include "Team.h"
#include <cmath>
#include "geomutils.h"

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

#define MINBALLSPEED 10
#define MAXLOOKAHEADTIME 2

Goalkeeper::Goalkeeper(QObject* p, Robot* r, qreal s)
	: Tactic(p,r),
	goto_(new Goto(this, r, 0.0, 0.0, 0.0, s, true))
{
	((Steer *)goto_)->setLookPoint(stage()->ball());
	goto_->setPoint(robot()->goal());
	this->pushState(goto_);//this is important
	//goto_->setIgnoreBrake();
}

bool Goalkeeper::busy()
{
	return true;
}

void Goalkeeper::step()
{
	//shortcuts:
	//TODO: make an architecture which naturally favors these shortcuts
	Stage &stage(*this->stage());
	Robot &robot(*this->robot());
	Goal &goal(*robot.goal());
	//Team &myTeam(*robot.team());//unused
	//Team &enemyTeam(*robot.enemyTeam());//unused
	Ball &ball(*stage.ball());

	//TODO: if ball is inside area and is slow, kick/pass it far far away

	/// Build the home line

	//This angle is how much inside the goal the goalkeeper must be
	//when 0 it's completely outside, when 90, it's half inside, when 180 it's completely inside
	//values greater than 90 don't make much sense
	const qreal angle(0);//TODO parametrize this

	//Auxiliar lines to translate the goal line ends
	Line l1 = Line::fromPolar(robot.body().radius(), goal.x() > 0 ? 180 - angle : angle);
	Line l2 = Line::fromPolar(robot.body().radius(), goal.x() > 0 ? 180 + angle : -angle);

	Line homeline(l1.translated(goal.p1()).p2(), l2.translated(goal.p2()).p2());

	/// Findout where in the homeline should we stay

	//watch the enemy
	//TODO: get the chain of badguys, (badguy and who can it pass to)
	//Robot &badguy = *enemyTeam.getClosestPlayerToBall();//unused

	//watch our attacker
	//TODO: if self then we should kick/pass the ball
	//Robot &goodguy = *myTeam.getClosestPlayerToBall();//unused

	//if the ball is moving fast* torwards the goal, defend it: THE CATCH
	//*: define fast
	goto_->setPoint(goal);
	Line ballPath(Line(Point(0, 0), ball.speed().toPointF()).translated(ball));
	ballPath.setLength(ball.speed().length() * MAXLOOKAHEADTIME);
	Point importantPoint;
	if(ball.speed().length() >= MINBALLSPEED && ballPath.intersect(homeline, &importantPoint) == Line::BoundedIntersection) {
		goto_->setPoint(importantPoint);
		goto_->step();
		return;
	}

	//if the badguy has closest  reach to the ball than watch it's orientation
	//TODO

	//otherwise try to close the largest gap
	//TODO

	//continue stepping the last strategy
	goto_->step();
}