#include "PenaltyThem.h"
#include "Tactic.h"
#include "qmath.h"
#include "Goal.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

PenaltyThem::PenaltyThem(QObject* parent, Team* team ,Stage* stage, Robot* gk)
	: Play(parent, team, stage)
{
	player_[0] = new Goalkeeper(this, gk, 3000);
	for(int i = 0; i < team->size(); i++)
		if(team->at(i)->id()!=gk->id())
			gotos.push_back(new Goto(this, team->at(i), 0, 0, 0, 3000, false));
}

PenaltyThem::~PenaltyThem()
{
	for(int i=0; i<gotos.size(); i++)
		delete gotos.at(i);
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
void PenaltyThem::step()
{
	Team* team = this->team_;
	Stage* stage = this->stage_;
	Ball* ball = stage->ball();
	Goal* myGoal = team->goal();
	qreal xPenalty = myGoal->penaltyLine();

	if(gotos.size()>0)
		gotos.at(0)->setPoint(xPenalty, 0);
	for(int i=2; i<gotos.size(); i+=2){
		gotos.at(i-1)->setPoint(xPenalty, +(i-1)*200);
		gotos.at(i)->setPoint(xPenalty, -(i-1)*200);
	}

	player_[0]->step();	
	for(int i=0; i<gotos.size(); i++){
		if(xPenalty>0)
			gotos.at(i)->setOrientation(0);
		else
			gotos.at(i)->setOrientation(M_PI);
		gotos.at(i)->step();
	}
}


