#include "Goalkeeper.h"
#include "Steer.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include "Team.h"
#include <cmath>
#include <QLineF>

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Goalkeeper::Goalkeeper(QObject* p, Robot* r, qreal s)
	: Tactic(p,r),
	goto_(new Goto(this, r, 0.0, 0.0, 0.0, s, true))//, kickTo_(new KickTo(this, r))//, getBall_(new GetBall(this, r, 1000))
{
	//goto_->setSpeed(speed);
	this->pushState(goto_);
	//skills.append(goto_);//this is important
	//goto_->setIgnoreBrake();
	//skills.append(kickTo_);
	//skills.append(getBall_);
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
	Team &myTeam(*robot.team());
	Team &enemyTeam(*robot.enemyTeam());
	Ball &ball(*stage.ball());

	//TODO: if ball is inside area and is slow, kick/pass it far far away

	/// Build the home line

	//This angle is how much inside the goal the goalkeeper must be
	//when 0 it's completely outside, when 90, it's half inside, when 180 it's completely inside
	//values greater than 90 don't make much sense
	const qreal angle(45);//TODO parametrize this

	//Auxiliar lines to translate the goal line ends
	QLineF l1 = QLineF::fromPolar(robot.body().radius(), goal.x() > 0 ? 180 - angle : angle);
	QLineF l2 = QLineF::fromPolar(robot.body().radius(), goal.x() > 0 ? 180 + angle : -angle);

	QLineF homeline(l1.translated(goal.p1()).p2(), l2.translated(goal.p2()).p2());

	/// Findout where in the homeline should we stay

	//watch the enemy
	//TODO: get the chain of badguys, (badguy and who can it pass to)
	Robot &badguy = *enemyTeam.getClosestPlayerToBall();

	//watch our attacker
	//TODO: if self then we should kick/pass the ball
	Robot &goodguy = *myTeam.getClosestPlayerToBall();

	/// TODO

	/// The backup plan
	QLineF ballToGoal(ball, goal);//to the center of the goal
	QPointF target;
	if(homeline.intersect(ballToGoal, &target) == QLineF::BoundedIntersection)
		goto_->setPoint(target);
	else
		goto_->setPoint(homeline.pointAt(target.y() > 0 ? 1 : 0));
	goto_->step();
}