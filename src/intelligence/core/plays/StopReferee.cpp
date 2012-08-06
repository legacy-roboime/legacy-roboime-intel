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
	//usei team->at(0), mas tanto faz pq a tatica é associada dinamicamente ao robo
	player_[1] = new Blocker(this, team->at(0), -deltaTeta);
	player_[2] = new Blocker(this, team->at(0), 0);
	player_[3] = new Blocker(this, team->at(0), deltaTeta);
	//usei team->enemyTeam()->at(0), mas tanto faz pq o enemy eh associada dinamicamente ao robo
	for(int i = 4; i < team->size(); i++)
		player_[i] = new Defender(this, team->at(0), team->enemyTeam()->at(0), 600, 3000);
}

StopReferee::~StopReferee()
{
}

//Play que controla os robos quando o juiz indica Stop
//Os robos são associados as suas táticas dinamicamente
//O robo goleiro não é dinamico por regra, sendo o robo passado no construtor
//Os 3 robos, mais proximos a bola, (exeto o goleiro) da nossa equipe sao usado para blocker (eles sao ordenados entre eles por id, para nao ficar instavel e chaveando)
//Os 2 robos restantes são ordenados por id e defendem dos dois oponentes mais próximos ao nosso gol
void StopReferee::step()
{
	Team* team = this->team_;
	Stage* stage = this->stage_;
	Ball* ball = stage->ball();
	//Goal* myGoal = team->goal();//unused

	map<qreal, Robot*> nearTeam = stage->getClosestPlayersToPoint(team, (Point*)ball); //Nossos robos por ordem de proximidade da bola (exceto o goleiro)
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
		it1++;
	}
	//2 defenders
	it1 = defenders.begin();
	it = nearEnemy.begin();
	for(int i=4; i<6 && it1!=defenders.end() && it!=nearEnemy.end(); i++){
		player_[i]->setRobot((*it1).second);
		((Defender*)player_[i])->setEnemy((*it).second);
		((Defender*)player_[i])->reset();
		it1++;
		it++;
	}

	for(int i = 0; i < team->size(); i++){
		player_[i]->step();	
	}
}


