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

	init = false;

	this->speed = speed;
}

void AutoRetaliate::step(){
	Team* team = this->team_;
	Goal* myGoal = team->goal();

	if(!init && myGoal->width()>0){
		Goal* goal = this->team()->goal();
		cover1 = new Point(goal->x(), goal->y() + goal->width()/3);
		cover2 = new Point(goal->x(), goal->y() - goal->width()/3);
		cover3 = new Point(goal->x(), goal->y());
		//usei team->enemyTeam()->at(0), mas tanto faz pq o enemy eh associada dinamicamente ao robo
		for(int i = 3; i < this->team()->size(); i++){
			Point* p;
			if(i==3)
				p = cover1;
			else if(i==4)
				p = cover2;
			else
				p = cover3;
			player_[i] = new Defender(this, this->team()->at(0), this->team()->enemyTeam()->at(0), p, 600, speed);
		}

		init = true;
	}
	else if(init){
		Stage* stage = this->stage_;
		Ball* ball = stage->ball();

		map<qreal, Robot*> close = stage->getClosestPlayersToBall(team);
		map<int, Robot*> ids;
		map<qreal, Robot*> enemys = stage->getClosestPlayersToPoint(team->enemyTeam(), (Point*)team->goal());

		//Attacker e Blocker
		int i=0;
		map<qreal, Robot*>::iterator it1 = close.begin();
		for(int j=0; j<close.size(); j++){
			if((*it1).second->id() == player_[0]->robot()->id()){
				it1++;
				continue;
			}

			Robot* robot = (*it1).second;
			if(i == 0){
				player_[1]->setRobot(robot);
				player_[1]->step();
			}
			else if(i == 1){
				player_[2]->setRobot(robot);
				player_[2]->step();
			}
			else
				ids[robot->id()] = robot;

			i++;
			it1++;
		}

		//Defenders enemys
		map<qreal, Robot*>::iterator it3 = enemys.begin(); 

		//Defenders
		map<int, Robot*>::iterator it2 = ids.begin();
		for(int i=3; i<team->size() && it2!=ids.end(); i++){
			player_[i]->setRobot((*it2).second);

			if(it3!=enemys.end() && (*it3).second != NULL){
				((Defender*)player_[i])->setEnemy((*it3).second);
				it3++;
			}
			else{
				((Defender*)player_[i])->setEnemy(ball);
				((Defender*)player_[i])->reset();
			}

			player_[i]->step();

			it2++;
		}

		//Goleiro
		if(player_[0]->robot())
			player_[0]->step();
	}
}
