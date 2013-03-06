#include "LibIntelligence.h"
#include "Stage.h"
#include "Robot.h"
#include "Skills.h"
#include "Goal.h"
#include "GoalSwitcheroo.h"

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

GoalSwitcheroo::GoalSwitcheroo(QObject *parent, Robot* slave, qreal s)
	: Tactic(parent, slave),
    goto_(new Goto(this, slave)),
    atacando(true)
{
	goto_->setSpeed(10000);
	qreal bluegoalx = 3000;
	qreal bluegoaly = 0;
	goto_->setAll(bluegoalx, bluegoaly, s);
	this->pushState(goto_);
	//skills.append(goto_);
}

GoalSwitcheroo::~GoalSwitcheroo()
{
}

void GoalSwitcheroo::step()
{
	//Stage* stage = this->stage();//unused
	Robot* robot = this->robot();

	//qreal BlueGoalX = -3000;
	//qreal BlueGoalY = 0;
	//
	//qreal YellGoalX = 3000;
	//qreal YellGoalY = 0;
	//printf("PosX= %f PosY=%f\n",robot->x(), robot->y());
	if(robot->x() >= 800 && atacando){
		atacando = false;
		goto_->setPoint(-3000,0);
		goto_->step();
	}
	else if(robot->x() <= -800 && !atacando ){
		atacando = true;
		goto_->setPoint(3000,0);
		goto_->step();
	}
	else{
		goto_->step();
	}
}
