#include <iostream>
#include <QStateMachine>
#include <QState>
#include "Intelligence.h"
#include "Robot.h"
#include "Goal.h"
#include "Robots.h"
#include "CommanderTx.h"
#include "CommanderTxOld.h"
#include "CommanderSim.h"
#include "UpdaterVision.h"
#include "UpdaterReferee.h"
#include "Ball.h"
#include "KalmanFilters.h"
#include "UpdateStageGeometry.h"
#include "UpdaterReferee.h"
#include <QThread>
#include <QCoreApplication>

using namespace LibIntelligence;

Intelligence::Intelligence(QObject *parent)
	: QObject(parent),
	comB(new CommanderSim(this)),
	comB2(new CommanderTxOld(this)),
	comY(new CommanderSim(this)),
	upd(new UpdaterVision(this)),
	filter(new KalmanFilters(parent, upd->updates())),
	updReferee(new UpdaterReferee(this))
{
	sta = new Stage();
	myTeam = sta->blueTeam();
	enemyTeam = sta->yellowTeam();
	ball = sta->ball();
	upd->add(ball);
	upd->add(sta);
	updReferee->add(sta);

	for(quint8 i=0; i<5; i++) {
		//if(i==0) {
		myTeam->push_back(new Robot(Robots::RobotIME2011(myTeam, i, i, BLUE)));
		comB->add(myTeam->last());
		upd->add(myTeam->last());

		enemyTeam->push_back(new Robot(Robots::RobotIME2011(enemyTeam, i, i, YELLOW)));
		comY->add(enemyTeam->last());
		upd->add(enemyTeam->last());

		//if(i == 0) {
		//	comB2->add(enemyTeam.last());
		//} else {
			comB2->add(myTeam->last());
		//}
	}

	//myTeam->at(0)->setPatternId(1);
	//myTeam->at(1)->setPatternId(1);
	//myTeam->at(2)->setPatternId(1);
	//myTeam->at(3)->setPatternId(1);
	//myTeam->at(3)->setPatternId(1);
	//enemyTeam[0]->setPatternId(4);

	//set the kicker if it is not working
	//myTeam[0]->kicker().setNotWorking();
		//myTeam[1]->kicker().setNotWorking();
	//myTeam[2]->kicker().setNotWorking();
	//myTeam[3]->kicker().setNotWorking();
	//myTeam[4]->kicker().setNotWorking();

	controller = new Controller(this, myTeam->at(3), 1, 500); //controle no referencial do campo
	//controller = new Tactics::Controller2(this, myTeam->at(3), 1, 1000); //controle no referencial do robo
	//gotoold = new GotoOld(this, myTeam[3], 0.0, 0.0);
	//skill1 = new DriveTo(this, myTeam->at(1), -3.14/2., QPointF(0,0), 1000.);

	skill1 = new Move(this, myTeam->at(4), 500, 0, 0.);//Goto(this, myTeam->at(3), 1000, 0, 0, 500, true);//SteerToBall(this, myTeam->at(3), 0, 0);//
	skill2 = new SampledKick(this, myTeam->at(1), enemyTeam->goal(), true, 0, 1, 500, false);
	skill3 = new SampledDribble(this, myTeam->at(1), enemyTeam->at(1), true, 1, 1, 1000);

	tactic = new Attacker(this, myTeam->at(1), 1000);
	machine = new Zickler43(this, myTeam->at(3), 1000, true);

	halt = new Plays::Halt(this, myTeam, sta);
	stopReferee = new Plays::StopReferee(this, myTeam, sta);
	cbr2011 = new Plays::CBR2011(this, enemyTeam, sta);
	//play = new Plays::Minmax2(this, myTeam, sta);
	attacker =  new AttackerMinMax2(this, myTeam->at(1), 3000);
	//play = new Plays::BGT(this, myTeam, sta);
	play = new Plays::Minmax2(this, myTeam, sta);
	//freeKickThem = new Plays::FreeKickThem(this, &myTeam, sta);

	player0 = new Goalkeeper(this, enemyTeam->at(0),1000);
	player1 = new Defender(this, enemyTeam->at(1), 0, 1000);
	player2 = new Defender(this, enemyTeam->at(2), 0, 1000);
	player3 = new Defender(this, enemyTeam->at(3), 0, 1000);
	player4 = new Attacker(this, enemyTeam->at(4), 1000);
	
#ifndef SOCCER_DEBUG
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(50.);//valor que tá no transmission da trunk para realTransmission //30.);//
#endif
	
	//connect(upd, SIGNAL(receiveUpdate()), filter, SLOT(step()));
}

Intelligence::~Intelligence()
{
	
}

void Intelligence::update() {
	//QTime Tempo;
	//Tempo.start();

	upd->step();
	upd->apply();
	//filter->updates()->apply(upd);
	/*updReferee->step();
	updReferee->apply();

	//Commands Referee
	char cmd = Stage::getCmdReferee();//sta->getCmdReferee(); //TODO: utilizar o getCmdReferee

	//printf("JUIZ: %c\n",cmd);

	static char lastCmd = 'H';
	static qreal bx, by;
	qreal dist=-1;

	dist = sqrt(pow(bx-ball->x(),2) + pow(by-ball->y(),2));

	if(lastCmd != cmd){
		
		lastCmd = cmd;
		bx = ball->x();
		by = ball->y();
	}

	TeamColor usColor = myTeam->color();

	if(cmd == 'f' && usColor == TeamColor::YELLOW)
		cbr2011->step();
	else if(cmd == 'f' && usColor == TeamColor::BLUE){
		if(dist<100)
			stopReferee->step();
		else
			cbr2011->step();
	}
	else if(cmd == 'F' && usColor == TeamColor::YELLOW){
		if(dist<100)
			stopReferee->step();
		else
			cbr2011->step();
	}
	else if(cmd == 'F' && usColor == TeamColor::BLUE)
		cbr2011->step();

	else if(cmd == 'i' && usColor == TeamColor::YELLOW)
		cbr2011->step();
	else if(cmd == 'i' && usColor == TeamColor::BLUE){
		if(dist<100)
			stopReferee->step();
		else
			cbr2011->step();
	}
	else if(cmd == 'I' && usColor == TeamColor::YELLOW){
		if(dist<100)
			stopReferee->step();
		else
			cbr2011->step();
	}
	else if(cmd == 'I' && usColor == TeamColor::BLUE)
		cbr2011->step();

	else if(cmd == 'H')
		halt->step();
	else if(cmd == 'S')
		stopReferee->step();
	else if(cmd == ' ')
		cbr2011->step();
	else if(cmd == 's')
		cbr2011->step();

	else if(cmd == '1' || cmd == 'h' || cmd == '2' || cmd == 'o' || cmd == 'O' || cmd == 'a')
		stopReferee->step();

	else if(cmd == 'k' || cmd == 'K')
		stopReferee->step();

	else if(cmd == 't' || cmd == 'T')
		halt->step();

	else if(cmd == 'z' || cmd == 'g' || cmd == 'G' || cmd == 'd' || cmd == 'D' || cmd == 'y' || cmd == 'Y' || cmd == 'r' || cmd == 'R' || cmd == 'c')
		stopReferee->step();

	else if(cmd == 'p' && usColor == TeamColor::YELLOW)
		stopReferee->step();
	else if(cmd == 'p' && usColor == TeamColor::BLUE)
		stopReferee->step();
	else if(cmd == 'P' && usColor == TeamColor::YELLOW)
		stopReferee->step();
	else if(cmd == 'P' && usColor == TeamColor::BLUE)
		stopReferee->step();

	else
		cbr2011->step();*/

	//cout << "X: " << myTeam->at(3)->x() << endl;
	//controller->step();
	//gotoold->step();
	//cbr2011->step();
	//machine->step();
	//((Skill*)machine->configuration().toList().at(1))->step();
	//player0->step();
	//player1->step();
	//player2->step();
	//player3->step();
	//player4->step();
	play->step();
	//attacker->step();
	//skill1->step();
	//if(!skill3->busy()) cout << "DESOCUPADO!!!" << endl;
	//else cout << "OCUPADO!!!" << endl;
	//tactic->step();
	//skill1->step();
	//tactic->step();

	comB->step();
	//comB2->step();
	//comY->step();
	((CommanderSim*)comB)->send();
	//((CommanderTxOld*)comB2)->send();
	//((CommanderSim*)comY)->send();

	//int duracao=Tempo.elapsed();
	//printf("TEMPO: %i ms\n",duracao);

}
