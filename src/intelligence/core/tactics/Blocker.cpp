#include "Blocker.h"
#include "Steer.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include <cmath>
#include "mathutils.h"

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Blocker::Blocker(QObject* p, Robot* r, qreal angle, qreal speed, qreal dist)
	: Tactic(p,r),
	goto_(new Goto(this, r, 0, 0, 0, speed)),
	angle_(angle),
	dist_(dist)
{
	this->pushState(goto_);
}

//Tatica na qual o robo bloqueia o gol para impedir que o robo oponente possa executar um chute ao gol. 
//Ele cria um arco em relacao a bola, onde angle=0 fica na linha entre a bola e o meu gol
void Blocker::step()
{
	//Elements
	Stage* stage = this->stage();
	Robot* robot = this->robot();
	Goal* myGoal = robot->goal();
	Ball* ball = stage->ball();

	Line target = Line(ball->x(), ball->y(), myGoal->x(), myGoal->y());
	target.setLength(dist_);
	qreal angle = target.angle();
	target.setAngle(angle + (angle_ * 180. / M_PI));
	qreal x = target.p2().x();
	qreal y = target.p2().y();
	goto_->setPoint(x, y);
	goto_->setOrientation(M_PI + angle * M_PI / 180.);
	goto_->step();
}

void Blocker::setSpeed(qreal speed)
{
	this->goto_->setSpeed(speed);
}
