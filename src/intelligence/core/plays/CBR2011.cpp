#include "CBR2011.h"
#include "Ball.h"
#include "Stage.h"
#include "Goal.h"

//#define LOGGING

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

CBR2011::CBR2011(QObject *parent, Team* team ,Stage* stage)
	: log("..\\log.dat"),
	Play(parent,team,stage)
{
	log.open(QIODevice::WriteOnly);

	Team* enemyTeam = team->enemyTeam();

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
			player_[i] = new Defender(this, team->at(i), enemyTeam->at(i), 600, 3000);
			break;
		case 2:
			//player_[i] = new Attacker(this, team->at(i), 1000);
			player_[i] = new Defender(this, team->at(i), enemyTeam->at(i), 600, 3000);
			//player_[i] = new Goalkeeper(this, team->at(i),10);
			break;
		case 3:
			//player_[i] = new Attacker(this, team->at(i), 1000);
			player_[i] = new Defender(this, team->at(i), enemyTeam->at(i), 600, 3000);
			//player_[i] = new Goalkeeper(this, team->at(i),10);
			break;
		case 4:
			player_[i] = new Blocker(this, team->at(i), 0, 1000);
			//player_[i] = new Defender(this, team->at(i), 0, 1000);
			//player_[i] = new Goalkeeper(this, team->at(i),10);
			break;
		case 5:
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
	Ball* ball = stage->ball();
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

#ifdef LOGGING
	QDataStream out(&log);   // write the data
	
	Team* yellowTeam = stage->yellowTeam();
	Team* blueTeam = stage->blueTeam();
	QString str("3 0 ");
	for(int i=0; i<5; i++){
		Robot* robot = yellowTeam->at(i);
		str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speed().x()) + " " + QString::number(robot->speed().y()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
	}
	//cout << str.toStdString() << endl;
	for(int i=0; i<5; i++){
		Robot* robot = blueTeam->at(i);
		str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speed().x()) + " " + QString::number(robot->speed().y()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
	}
	str += QString::number(ball->x()) + " " + QString::number(ball->y()) + " " + QString::number(ball->speed().x()) + " " + QString::number(ball->speed().y()) + " " + QString::number(0) + "\n";
	out << str;
	log.flush();
#endif
}
