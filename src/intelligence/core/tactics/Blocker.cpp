#include "Blocker.h"
#include "Steer.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include <cmath>
#include <QLineF>

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Blocker::Blocker(QObject* p, Robot* r, qreal angle, qreal speed)
	: Tactic(p,r),
	goto_(new Goto(this, r)),
	angle_(angle),
	speed(speed)
{
	this->pushState(goto_);
	//skills.append(goto_);//this is important
}

//Tatica na qual o robo bloqueia o gol para impedir que o robo oponente possa executar um chute ao gol. 
//Ele cria um arco em relacao a bola, onde angle=0 fica na linha entre a bola e o meu gol
void Blocker::step()
{
	//Elements
	Stage* stage = this->stage();
	Robot* robot = this->robot();
	Goal* myGoal;
	Ball* ball = stage->ball();
	//printf("bolax= %f bolay=%f\n",ball->x(), ball->y());
	
	//Get My Goal 
	//TODO: classe robot deve ter um metodo para pegar o meu gol e o gol oponente
	if(robot->color() == BLUE)
		myGoal = stage->blueGoal();
	else
		myGoal = stage->yellowGoal();

	QLineF target = QLineF(ball->x(), ball->y(), myGoal->x(), myGoal->y());
	target.setLength(700);
	qreal angle = target.angle();
	target.setAngle(angle + (angle_ * 180. / M_PI));
	qreal x = target.p2().x();
	qreal y = target.p2().y();
	goto_->setPoint(x, y);
	goto_->setSpeed(speed);
	goto_->setOrientation(M_PI - target.angle() * M_PI / 180.);
	goto_->step();
}
