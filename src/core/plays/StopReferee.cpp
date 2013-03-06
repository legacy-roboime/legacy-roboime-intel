#include "StopReferee.h"
#include "Tactic.h"
#include "qmath.h"
#include "Goal.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

StopReferee::StopReferee(QObject* parent, Team* team ,Stage* stage, Robot* gk)
	: Play(parent, team, stage)
{
	qreal deltaTeta = (23*3.14)/180.;
	player_[0] = new Goalkeeper(this, gk, 3000);
	((Goalkeeper*)player_[0])->setAggressive(false);
	//usei team->at(0), mas tanto faz pq a tatica � associada dinamicamente ao robo
	player_[1] = new Blocker(this, team->at(0), 0);
	player_[2] = new Blocker(this, team->at(0), -deltaTeta);
	player_[3] = new Blocker(this, team->at(0), deltaTeta);
	//usei team->enemyTeam()->at(0), mas tanto faz pq o enemy eh associada dinamicamente ao robo
	for(int i = 4; i < this->team()->size(); i++){
		player_[i] = new Defender(this, this->team()->at(0), this->team()->enemyTeam()->at(0), team->goal(), 600, 3000);
	}

	cover1 = new Object(0, 0);
	cover2 = new Object(0, 0);

	init = false;
}

StopReferee::~StopReferee()
{
}

void StopReferee::setGoalkeeper(Robot* gk)
{
    player_[0]->setRobot(gk);
}

//Play que controla os robos quando o juiz indica Stop
//Os robos s�o associados as suas t�ticas dinamicamente
//O robo goleiro n�o � dinamico por regra, sendo o robo passado no construtor
//Os 3 robos, mais proximos a bola, (exeto o goleiro) da nossa equipe sao usado para blocker (eles sao ordenados entre eles por id, para nao ficar instavel e chaveando)
//Os 2 robos restantes s�o ordenados por id e defendem dos dois oponentes mais pr�ximos ao nosso gol
void StopReferee::step()
{
	Team* team = this->team_;
	Goal* myGoal = team->goal();

	if(myGoal->width()>0){
		cover1->setX(myGoal->x());
		cover1->setY(myGoal->y() + myGoal->width()/4);
		cover2->setX(myGoal->x());
		cover2->setY(myGoal->y() - myGoal->width()/4);
		for(int i = 4; i < this->team()->size(); i++){
			Object* p;
			if(i==4)
				p = cover1;
			else
				p = cover2;
			((Defender*)player_[i])->setCover(p);
		}

		init = true;
	}

	if(init){
		Stage* stage = this->stage_;
		Ball* ball = stage->ball();


		map<qreal, Robot*> nearTeam = stage->getClosestPlayersToBallThatCanKick(team); //Nossos robos por ordem de proximidade da bola (exceto o goleiro)
		map<int, Robot*> blockers;
		map<int, Robot*> defenders;
		map<qreal, Robot*> nearEnemy = stage->getClosestPlayersToPoint(team->enemyTeam(), myGoal->x(), myGoal->y()); //oponentes ordenados por proximidade do nosso gol

		//Tirando o Goleiro
		map<qreal, Robot*>::iterator it = nearTeam.begin();
		for(int i=0; i<nearTeam.size(); i++){
			Robot* r = (*it).second;
			if(r->id() == player_[0]->robot()->id()){
				nearTeam.erase(it);
				break;
			}
			it++;	
		}

		//3 blockers ordenados por id
		it = nearTeam.begin();
		for(int i=0; i<3 && it!=nearTeam.end(); i++){
			Robot* r = (*it).second;
			blockers[r->id()]=r;
			it++;
		}
		//2 defenders ordenados por id
		for(int i=0; i<2 && it!=nearTeam.end(); i++){
			Robot* r = (*it).second;
			defenders[r->id()]=r;
			it++;
		}

		//3 blockers
		map<int, Robot*>::iterator it1 = blockers.begin();
		for(int i=1; i<4 && it1!=blockers.end(); i++){
			player_[i]->setRobot((*it1).second);
			player_[i]->step();
			it1++;
		}

		//2 defenders
		it1 = defenders.begin();
		it = nearEnemy.begin();
		for(int i=4; i<6 && it1!=defenders.end(); i++){
			player_[i]->setRobot((*it1).second);

			if( it!=nearEnemy.end() ){
				((Defender*)player_[i])->setEnemy((*it).second);
				it++;
			}
			else
				((Defender*)player_[i])->setEnemy(ball);

			((Defender*)player_[i])->reset();	
			((Defender*)player_[i])->step();
			it1++;
		}

		//Goleiro
		if(player_[0]->robot())
			player_[0]->step();
	}
}


