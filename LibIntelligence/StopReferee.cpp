#include "StopReferee.h"
#include "Tactic.h"
#include "qmath.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

StopReferee::StopReferee(QObject* parent, Team* team ,Stage* stage)
	: Play(parent, team, stage)
{
	qreal deltaTeta = (30*3.14)/180.;
	for( int i = 0; i < team->size(); i++){
		switch(i){
			case 0:
				//player_[i] = new Blocker(this,team->at(i),0);
				player_[i] = new Goalkeeper(this,team->at(i), 700);
				break;
			case 1:
				player_[i] = new Blocker(this,team->at(i),0);
				//player_[i] = new Goalkeeper(this,team->at(i), 700);
				//player_[i] = new Goalkeeper(this,team->at(i), 700);
				break;
			case 2:
				player_[i] = new Blocker(this,team->at(i),-deltaTeta);
				break;
			case 3:
				//player_[i] = new Defender(this,team->at(i), 0, 1000);
				player_[i] = new Blocker(this,team->at(i),+deltaTeta);
				break;
			case 4:
				player_[i] = new Blocker(this,team->at(i),+2*deltaTeta);
				break;
		}
	}
}

StopReferee::~StopReferee()
{
}

void StopReferee::step()
{
	Stage* stage = this->stage_;
	Team* team = this->team_;
	//9cm is the Robot's radius and 50cm is the distance allowed between the ball and any player
	//but I used a exeperimental value
	qreal deltaTeta = (30*3.14)/180.;
	for( int i = 0; i < team->size(); i++){
		player_[i]->setRobot( team->at(i) );
		player_[i]->step();
	}
}


