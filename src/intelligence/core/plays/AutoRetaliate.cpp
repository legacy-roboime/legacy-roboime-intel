#include "AutoRetaliate.h"
#include "Ball.h"
#include "Stage.h"
#include "Goal.h"
#include "Tactics.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

AutoRetaliate::AutoRetaliate(QObject *parent, Team* team, Stage* stage, Robot* gk, qreal speed)
	: Play(parent,team,stage)
{
	player_[0] = new Goalkeeper(this, gk, speed);
	//usei team->at(0), mas tanto faz pq a tatica é associada dinamicamente ao robo
	player_[1] = new Zickler43(this, team->at(0), speed, true);
	player_[2] = new Blocker(this, team->at(0), 0, speed);
	for(int i = 3; i < team->size(); i++)
		player_[i] = new Defender(this, team->at(0), team->enemyTeam()->at(i), speed);
}

void AutoRetaliate::step(){
	Team* team = this->team_;
	Stage* stage = this->stage_;
	Ball* ball = stage->ball();
	Goal* myGoal = team->goal();

	map<qreal, Robot*> m = stage->getClosestPlayersToBall(team);
	map<qreal, Robot*>::iterator it = m.begin();
	int i=0;
	while(i<m.size()-1){
		if((*it).second->id() == player_[0]->robot()->id()){
			it++;
			continue;
		}

		Robot* robot = (*it).second;
		if(i == 0)
			player_[1]->setRobot(robot);
		else if(i == 1)
			player_[2]->setRobot(robot);
		else if(i == 2)
			player_[3]->setRobot(robot);
		else if(i == 3)
			player_[4]->setRobot(robot);
		else if(i == 4)
			player_[5]->setRobot(robot);

		i++;
		it++;
	}

	for(int i = 0; i < team->size(); i++){
		Tactic* t = player_[i];
		player_[i]->step();	
	}
}
