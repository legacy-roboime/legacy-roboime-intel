#include "FreeKickThem.h"
#include "Tactic.h"
#include "qmath.h"
#include "Stage.h"
#include "Ball.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

FreeKickThem::FreeKickThem(QObject* parent, Team* team ,Stage* stage)
	: Play(parent, team, stage)
{
	qreal deltaTeta = (30*3.14)/180.;
	for( int i = 0; i < team->size(); i++){
		switch(i){
			case 0:
				player_[i] = new Blocker(this,team->at(i),0,1000);
				break;
			case 1:
				player_[i] = new Goalkeeper(this,team->at(i), 1000);
				break;
			case 2:
				player_[i] = new Blocker(this,team->at(i),-deltaTeta,1000);
				break;
			case 3:
				player_[i] = new Defender(this,team->at(i), 0, 1000);
				break;
			case 4:
				player_[i] = new Blocker(this,team->at(i),deltaTeta,1000);
				break;
		}
	}
}

FreeKickThem::~FreeKickThem()
{
}

void FreeKickThem::step()
{
	Stage* stage = this->stage_;
	Team* team = this->team_;
	Ball* ball = stage->ball();

	static qreal bx = -9999.;
	static qreal by = -9999.;

	qreal dist = sqrt(pow(bx-ball->x(),2) + pow(by-ball->y(),2));

	if(dist > 100 && bx!=-9999. && by!=-9999.){
		Stage::setCmdReferee('s');
	}

	//9cm is the Robot's radius and 50cm is the distance allowed between the ball and any player
	//but I used a exeperimental value
	//qreal deltaTeta = (30*3.14)/180.;
	for( int i = 0; i < team->size(); i++){
		player_[i]->setRobot( team->at(i) );
		player_[i]->step();
	}
}