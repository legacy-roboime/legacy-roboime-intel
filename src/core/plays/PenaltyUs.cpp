#include "PenaltyUs.h"
#include "Tactic.h"
#include "qmath.h"
#include "Goal.h"


using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

PenaltyUs::PenaltyUs(QObject* parent, Team* team ,Stage* stage, Robot* pKicker, Robot* gk)
	: Play(parent, team, stage),
	penaltyKicker(pKicker)
{
	qreal deltaTeta = (0*3.14)/180.;
	
	player_[0] = new Blocker(this, pKicker, deltaTeta, 3000, 300);
	player_[1] = new Goalkeeper(this, gk, 3000);
	((Goalkeeper*)player_[1])->setAggressive(false);
	for(int i = 0; i < team->size(); i++)
		gotos.push_back(new Goto(this, team->at(i), 0, 0, 0, 3000, false));
}

void PenaltyUs::setPenaltyKicker(Robot* pk)
{
	this->penaltyKicker = pk;
	((Blocker*) player_[0])->setRobot(pk);
}

PenaltyUs::~PenaltyUs()
{
	for(int i=0; i<gotos.size(); i++)
		delete gotos.at(i);
}

void PenaltyUs::setGoalkeeper(Robot* gk)
{
    player_[1]->setRobot(gk);
}

/*
The ball:
is placed on the penalty mark

The robot taking the penalty kick:
is properly identified

The defending goalkeeper:
remains between the goalposts, touches its goal line, and faces outward of the goal, until the ball
has been kicked. It is allowed to move before the ball has been kicked, as long as its motion does not
break any of these constraints.

The robots other than the kicker are located:
inside the field of play
behind a line parallel to the goal line and 400 mm behind the penalty mark*/
void PenaltyUs::step()
{
	Team* team = this->team_;
	//Stage* stage = this->stage_;//SA: Unused variable
	//Ball* ball = stage->ball();//SA: Dead store and unused variable
	//Goal* myGoal = team->goal();//SA: Dead store and unused variable
	qreal xPenalty = team->enemyGoal()->penaltyLine();
	qreal xPenaltyPlus = xPenalty+(xPenalty>0?-300:300);

	if(gotos.size()>0)
		gotos.at(0)->setPoint(xPenaltyPlus, 0);
	for(int i=1; i<gotos.size(); i++){
		gotos.at(i)->setPoint(xPenaltyPlus, qPow(-1,i)*i*200);
	}

	//Para funcionar se o goleiro for o batedor
	player_[1]->step();	
	player_[0]->step();	
	for(int i=0; i<gotos.size(); i++){
		if(xPenalty>0)
			gotos.at(i)->setOrientation(0);
		else
			gotos.at(i)->setOrientation(M_PI);
		if(team->at(i)->id()!=penaltyKicker->id() && team->at(i)->id()!=((Goalkeeper*)player_[1])->robot()->id())
			gotos.at(i)->step();
	}
}


