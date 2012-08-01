#include "StopReferee.h"
#include "Tactic.h"
#include "qmath.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

StopReferee::StopReferee(QObject* parent, Team* team ,Stage* stage, Robot* gk)
	: Play(parent, team, stage)
{
	qreal deltaTeta = (17*3.14)/180.;
	player_[0] = new Goalkeeper(this, gk, 3000);
	//usei team->at(0), mas tanto faz pq a tatica é associada dinamicamente ao robo
	player_[1] = new Blocker(this, team->at(0), -deltaTeta, 3000);
	player_[2] = new Blocker(this, team->at(0), 0, 3000);
	player_[3] = new Blocker(this, team->at(0), deltaTeta, 3000);
	//usei team->enemyTeam()->at(0), mas tanto faz pq o enemy eh associada dinamicamente ao enemy
	for(int i = 4; i < team->size(); i++)
		player_[i] = new Defender(this, team->at(0), team->enemyTeam()->at(0), 600, 3000);
}

StopReferee::~StopReferee()
{
}

void StopReferee::step()
{
	Team* team = this->team_;
	Stage* stage = this->stage_;
	Ball* ball = stage->ball();
	Goal* myGoal = team->goal();

	map<int/*qreal*/, Robot*> ids1;// = stage->getClosestPlayersToBall(team);
	map<int, Robot*> ids;
	map<qreal, Robot*> enemys = stage->getClosestPlayersToPoint(team->enemyTeam(), (Point*)team->goal());

	for(int i=0; i<team->size(); i++){
		Robot* r = team->at(i);
		if(r->id() != player_[0]->robot()->id())
			ids1[r->id()] = r;
	}

	////3 Blocker
	int i=0;
	map</*qreal*/int, Robot*>::iterator it1 = ids1.begin();
	while(i<ids.size()-1){
		if((*it1).second->id() == player_[0]->robot()->id()){
			it1++;
			continue;
		}

		Robot* robot = (*it1).second;
		if(i == 0)
			player_[1]->setRobot(robot);
		else if(i == 1)
			player_[2]->setRobot(robot);
		else if(i == 2)
			player_[3]->setRobot(robot);
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
		//	if((*it3).second != NULL && stage->inField((const Object&)(*(*it3).second))){
		//		((Defender*)player_[4])->setEnemy((*it3).second);
		//	}
		//	else{
				((Defender*)player_[4])->setEnemy(ball);
				((Defender*)player_[4])->reset();
		//	}
			player_[4]->setRobot(robot);
		}
		else if(i == 1){
		//	if((*it3).second != NULL && stage->inField((const Object&)(*(*it3).second))){
		//		((Defender*)player_[5])->setEnemy((*it3).second);
		//	}
		//	else{
				((Defender*)player_[5])->setEnemy(ball);
				((Defender*)player_[5])->reset();
		//	}
			player_[5]->setRobot(robot);
		}

		i++;
		it2++;
		//it3++;
	}

	for(int i = 0; i < team->size(); i++){
		player_[i]->step();	
	}
}


