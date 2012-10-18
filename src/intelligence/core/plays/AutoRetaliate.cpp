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
	//usei team->enemyTeam()->at(0), mas tanto faz pq o enemy eh associada dinamicamente ao robo
	for(int i = 3; i < this->team()->size(); i++){
		player_[i] = new Defender(this, this->team()->at(0), this->team()->enemyTeam()->at(0), team->goal(), 600, speed);
	}

	cover1 = new Point(0, 0);
	cover2 = new Point(0, 0);
	cover3 = new Point(0, 0);

	init = false;

	this->speed = speed;
}

void AutoRetaliate::setGoalkeeper(Robot* gk)
{
    player_[0]->setRobot(gk);
}

void AutoRetaliate::step(){
	Team* team = this->team_;
	Goal* myGoal = team->goal();

	if(player_[0]->robot()->isActive())
	{
		player_[0]->step();
	}
	else
	{
		map<qreal, Robot*> robotsCloseToGoal = stage()->getClosestPlayersToPoint(team, team->goal()->x(), team->goal()->y());
		map<qreal, Robot*>::iterator goalIter = robotsCloseToGoal.begin();
		for(int j=0; j<robotsCloseToGoal.size(); j++){
			Robot* r = goalIter.first;
		}
		for(int i=0; i<team->size(); i++)
		{
			if(team->at(i)->isActive())
			{
				player_[0]->setRobot(team->at(i));
				player_[0]->step();
				break;
			}
		}
	}

	if(myGoal->width()>0){
		Goal* goal = this->team()->goal();
		cover1->setX(goal->x());
		cover1->setY(goal->y() + goal->width()/3);
		cover2->setX(goal->x());
		cover2->setY(goal->y() - goal->width()/3);
		cover3->setX(goal->x());
		cover3->setY(goal->y());
		//usei team->enemyTeam()->at(0), mas tanto faz pq o enemy eh associada dinamicamente ao robo
		for(int i = 3; i < this->team()->size(); i++){
			Point* p;
			if(i==3)
				p = cover1;
			else if(i==4)
				p = cover2;
			else
				p = cover3;
			((Defender*)player_[i])->setCover(p);
		}

		init = true;
	}
	
	if(init){
		Stage* stage = this->stage_;
		Ball* ball = stage->ball();

		map<qreal, Robot*> close = stage->getClosestPlayersToBallThatCanKick(team);
		map<int, Robot*> ids;
		map<qreal, Robot*> enemys = stage->getClosestPlayersToPointThatCanKick(team->enemyTeam(), (Point*)team->goal());



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

		
	}
}
