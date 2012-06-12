#include "AutoRetaliate.h"
#include "Ball.h"
#include "Stage.h"
#include "Goal.h"
#include "Tactics.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

AutoRetaliate::AutoRetaliate(QObject *parent, Team* team, Stage* stage, qreal speed)
	: Play(parent,team,stage)
{
	player_[0] = new Goalkeeper(this, team->at(0), speed);
	player_[1] = new Zickler43(this, team->at(1), speed, true);
	player_[2] = new Blocker(this, team->at(2), 0, speed);
	for(int i = 3; i < team->size(); i++)
		player_[i] = new Defender(this, team->at(i), 0, speed);
}

void AutoRetaliate::step(){
	Team* team = this->team_;
	Stage* stage = this->stage_;
	Ball* ball = stage->ball();
	Goal* myGoal = team->goal();;

	Robot* newAtk = stage->getClosestPlayerToBallThatCanKick(team);
	Robot* oldAtk = player_[1]->robot();

	if(newAtk != player_[0]->robot()) {
		for(int i = 0; i < team->size(); i++) {
			if(player_[i]->robot() == newAtk) {
				player_[i]->setRobot(oldAtk);
				player_[1]->setRobot(newAtk);
			}
		}
	}

	for(int i = 0; i < team->size(); i++) {
		player_[i]->step();
	}
}
