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

#define MINBALLSPEED 100
#define MAXLOOKAHEADTIME 2
#define DOMINATIONRADIUS 135

Goalkeeper::Goalkeeper(QObject* p, Robot* r, qreal s)
	: Tactic(p,r),
    goto_(new Goto(this, r, 0.0, 0.0, 0.0, s, true)),
    kick(new SampledKick(this, r, r->enemyGoal())),
	isAggressive_(true)
{
	((Steer *)goto_)->setLookPoint(stage()->ball());
	goto_->setPoint(robot()->goal());
	this->pushState(goto_);//this is important
    this->pushState(kick);//this is important
	//goto_->setIgnoreBrake();
}

bool Goalkeeper::busy()
{
	return true;
}

Point Goalkeeper::pointToKeep()// defende o meio do maior buraco
{
    //TODO: this point should lay on the homeline
    //DONE
    Goal* goal = this->robot()->goal();
    qreal hwidth = goal->width() / 2;

    qreal initial = goal->y()-hwidth;

    Point p(goal->x(), initial);
    qreal holeSize = 0;

    Point centerMax;
    centerMax.setX(goal->x());
    --maxHoleSize;

    qreal delta = hwidth / 5;

    for (qreal t = initial; t < goal->y()+hwidth; t+=delta) {// 10 pontos
        if (isKickScored(Point(goal->x(), t))) {
            holeSize += delta;
        } else {
            if (holeSize > maxHoleSize) {
                maxHoleSize = holeSize;
                centerMax.setY(p.y() + holeSize / 2);
            }
            holeSize = 0;
            p.setY(t + delta);
        }
    }
    if (holeSize >= maxHoleSize) {
        maxHoleSize = holeSize;
        centerMax.setY(p.y() + holeSize / 2);
    }
    return centerMax;
}

//Checks if a kick from that position can score a goal.
bool Goalkeeper::isKickScored(Point kickPoint)
{
    Ball* ball = this->stage()->ball();
    Line ball_kickpoint = Line(*ball, kickPoint);
    Team* team = this->robot()->team();
    for (int i = 0; i < team->size(); i++) {
        Robot* obst = team->at(i);
        // Ignore itself so we don't start looping
        if (obst == robot()) continue;
        Point obstP(obst->x(), obst->y());
        Line ball_obst = Line(*ball, obstP);
        qreal dist = ball_kickpoint.distanceTo(obstP);
        qreal dotproduct = ball_kickpoint.dx() * ball_obst.dx() + ball_kickpoint.dy() * ball_obst.dy();
        if (dist < obst->body().radius() && dotproduct > 0)
            return false;
    }
    team = this->robot()->enemyTeam();
    for (int i = 0; i < team->size(); i++) {
        Robot* obst = team->at(i);
        Point obstP(obst->x(), obst->y());
        Line ball_obst = Line(*ball, obstP);
        qreal dist = ball_kickpoint.distanceTo(obstP);
        qreal dotproduct = ball_kickpoint.dx() * ball_obst.dx() + ball_kickpoint.dy() * ball_obst.dy();
        if (dist < obst->body().radius() && dotproduct > 0)
            return false;
    }
    return true;
}

qreal Goalkeeper::holeSize()
{
    return maxHoleSize;
}

bool Goalkeeper::isAggressive()
{
	return isAggressive_;
}

void Goalkeeper::setAggressive(bool aggro)
{
	isAggressive_ = aggro;
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

    /// Find out where in the homeline should we stay

	//watch the enemy
	//TODO: get the chain of badguys, (badguy and who can it pass to)
	//Robot &badguy = *enemyTeam.getClosestPlayerToBall();//unused
	if(isAggressive_)
	{
		// if we are the closest to the ball then kick it
		if (this->robot() == robot.stage()->getClosestPlayerToBallThatCanKick()) {
			return kick->step();
		}
	}

	//if the ball is moving fast* torwards the goal, defend it: THE CATCH
    //*: define fast
    //goto_->setPoint(goal);
	Line ballPath(Line(Point(0, 0), ball.speed().toPointF()).translated(ball));
	ballPath.setLength(ball.speed().length() * MAXLOOKAHEADTIME);
	Point importantPoint;
	if(ball.speed().length() >= MINBALLSPEED && ballPath.intersect(homeline, &importantPoint) == Line::BoundedIntersection) {
		goto_->setPoint(importantPoint);
		goto_->step();
		return;
	}

    //if the badguy has closest reach to the ball then watch it's orientation
    //TODO:
    Robot* dangerBot = robot.stage()->getClosestPlayerToBallThatCanKick();
    //If dangerBot is an enemy, we shall watch his orientation. If he's a friend, we move on to a more
    //appropriate strategy
    if(robot.color() != dangerBot->color() &&
            Line(dangerBot->x(),dangerBot->y(),ball.x(), ball.y()).length() < DOMINATIONRADIUS)
    {
        qreal dangerAngle = dangerBot->orientation();
        //Line starting from the dangerBot spanning twice the distance from the bot
        //to the goal with the desired orientation.
        Line dangerLine = Line::fromPolar(Line(dangerBot->x(),dangerBot->y(),
                             goal.x(),goal.y()).length() * 2, dangerAngle > 0 ? dangerAngle : 180 - dangerAngle)
                .translated(dangerBot->x(), dangerBot->y());
        //Intersection of the line with the goal base line
        Point blDangerPoint, hlDangerPoint;

        //If the intersection lies within the goal, we try intercepting!
        if(dangerLine.intersect(Line(goal.x(), goal.y() - goal.length()/2, goal.x(), goal.y() + goal.length()/2),
                                 &blDangerPoint)
            == Line::BoundedIntersection)
        {
            //Translating the danger point to the goalie's position on the home line
            if(dangerLine.intersect(homeline, &hlDangerPoint) == Line::BoundedIntersection)
            {
                goto_->setPoint(hlDangerPoint);
            }
            else
            {
                //If the translation is outside the home line, we put him in the closest endpoint of the line.
                if(Line(homeline.p1(), hlDangerPoint).length()< Line(homeline.p2(), hlDangerPoint).length())
                {
                    goto_->setPoint(homeline.p1());
                }
                else
                {
                    goto_->setPoint(homeline.p2());
                }
            }
            goto_->step();
            return;
        }
    }

    //Otherwise, try to close the largest gap
    Point blBestPoint = pointToKeep(), hlBestPoint;
    Line ballToBlBestPoint(ball, blBestPoint);
    if(homeline.intersect(ballToBlBestPoint, &hlBestPoint) == Line::BoundedIntersection)
    {
        goto_->setPoint(hlBestPoint);
    }
    else
    {
        if(Line(homeline.p1(), hlBestPoint).length()< Line(homeline.p2(), hlBestPoint).length())
        {
            goto_->setPoint(homeline.p1());
        }
        else
        {
            goto_->setPoint(homeline.p2());
        }
    }

	//continue stepping the last strategy
	goto_->step();
}
