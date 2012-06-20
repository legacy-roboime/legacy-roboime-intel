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
	//usei team->enemyTeam()->at(0), mas tanto faz pq o enemy eh associada dinamicamente ao enemy
	for(int i = 3; i < team->size(); i++)
		player_[i] = new Defender(this, team->at(0), team->enemyTeam()->at(0), 600, speed);
}

void AutoRetaliate::step(){
	Team* team = this->team_;
	Stage* stage = this->stage_;
	Ball* ball = stage->ball();
	Goal* myGoal = team->goal();

	map<qreal, Robot*> close = stage->getClosestPlayersToBall(team);
	map<int, Robot*> ids;
	map<qreal, Robot*> enemys = stage->getClosestPlayersToPoint(team->enemyTeam(), (Point*)team->goal());

	//Attacker e Blocker
	int i=0;
	map<qreal, Robot*>::iterator it1 = close.begin();
	while(i<close.size()-1){
		if((*it1).second->id() == player_[0]->robot()->id()){
			it1++;
			continue;
		}

		Robot* robot = (*it1).second;
		if(i == 0)
			player_[1]->setRobot(robot);
		else if(i == 1)
			player_[2]->setRobot(robot);
		else
			ids[robot->id()] = robot;

		i++;
		it1++;
	}

	//Defenders enemys
	map<qreal, Robot*>::iterator it3 = enemys.begin(); 

	//Defenders
	i=0;
	map<int, Robot*>::iterator it2 = ids.begin();
	while(i<ids.size()){
		Robot* robot = (*it2).second;
		if(i == 0){
			((Defender*)player_[3])->setEnemy((*it3).second);
			player_[3]->setRobot(robot);
		}
		else if(i == 1){
			((Defender*)player_[4])->setEnemy((*it3).second);
			player_[4]->setRobot(robot);
		}
		else if(i == 2){
			((Defender*)player_[5])->setEnemy((*it3).second);
			player_[5]->setRobot(robot);
		}

		i++;
		it2++;
		it3++;
	}

	Object* obj1 = ((Defender*)player_[3])->enemy();
	Object* obj2 = ((Defender*)player_[4])->enemy();
	Object* obj3 = ((Defender*)player_[5])->enemy();

	for(int i = 0; i < team->size(); i++){
		player_[i]->step();	
	}
}
