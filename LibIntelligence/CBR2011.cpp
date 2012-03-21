#include "CBR2011.h"
#include "Ball.h"
#include "Stage.h"
#include "Goal.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

CBR2011::CBR2011(QObject *parent, Team* team ,Stage* stage)
	: Play(parent,team,stage)
{
	for( int i = 0; i < team->size(); i++){
		switch(i){
			case 0:
				player_[i] = new Goalkeeper(this, team->at(i),1000);
				//player_[i] = new Attacker(this, team->at(i), 2000);
				//player_[i] = new Defender(this, team->at(1), 0, 1000);
				break;
			case 1:
				//player_[i] = new Attacker(this, team->at(i), 1000);
				//player_[i] = new Goalkeeper(this, team->at(i),10);
				player_[i] = new Defender(this, team->at(1), 0, 1000);
				break;
			case 2:
				//player_[i] = new Attacker(this, team->at(i), 1000);
				player_[i] = new Defender(this, team->at(i), 0, 1000);
				//player_[i] = new Goalkeeper(this, team->at(i),10);
				break;
			case 3:
				//player_[i] = new Attacker(this, team->at(i), 1000);
				player_[i] = new Defender(this, team->at(i), 0, 1000);
				//player_[i] = new Goalkeeper(this, team->at(i),10);
				break;
			case 4:
				player_[i] = new Attacker(this, team->at(i), 1000);
				//player_[i] = new Defender(this, team->at(i), 0, 1000);
				//player_[i] = new Goalkeeper(this, team->at(i),10);
				break;
		}
	}
}

CBR2011::~CBR2011()
{
}

void CBR2011::step(){
	Team* team = this->team_;
	Stage* stage = this->stage_;
	Ball* ball = this->stage_->ball();
	Goal* myGoal;

	if(team->color() == BLUE)
		myGoal = stage->blueGoal();
	else
		myGoal = stage->yellowGoal();
	
	/*Robot* newAtk = stage->getCloserPlayerToBallThatCanKick(team);
	Robot* oldAtk = player_[0]->robot();

	for( int i = 0; i < team->size(); i++){
		if(player_[i]->robot() == newAtk){
			player_[i]->setRobot(oldAtk);
			player_[0]->setRobot(newAtk);
		}
	}*/

	for( int i = 0; i < team->size(); i++){
		player_[i]->step();
	}
}
