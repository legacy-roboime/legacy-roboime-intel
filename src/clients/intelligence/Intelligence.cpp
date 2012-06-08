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

#define SIMU

struct IntelligenceCli : public QThread
{
	Intelligence *intel;
	IntelligenceCli(Intelligence *intel) : QThread(intel), intel(intel) {}

	void run() {
		qreal x, y, s;
		string command;
		while(true) {
			cout << "> ";
			cin >> command;
			switch(command[0]) {
			case 'p':
				cin >> x >> y;
				cout << "skill1->SetPoint(" << x << "," << y << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill1)->setPoint(x, y);
				intel->mutex.unlock();
				break;
			case 's':
				cin >> s;
				cout << "skill1->SetSpeed(" << s << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill1)->setSpeed(s);
				intel->mutex.unlock();
				break;
			default:
				cout << "Comando nao reconhecido." << endl;
			}
			cin.clear();
			cout.clear();
		}
	}
};

Intelligence::Intelligence(QObject *parent)
	: QObject(parent),
	comBSim(new CommanderSim(this)),
	comBTx(new CommanderTxOld(this)),
	comYSim(new CommanderSim(this)),
	comYTx(new CommanderTxOld(this)),
	upd(new UpdaterVision(this)),
	updSim(new UpdaterVision(this, "224.5.23.2", 11002)),
	filter(new KalmanFilters(parent, upd->updates())),
	updReferee(new UpdaterReferee(this)),
	sta(new Stage()),
	cli(new IntelligenceCli(this))
{
	myTeam = sta->blueTeam();
	enemyTeam = sta->yellowTeam();
	updReferee->add(sta);
	ball = sta->ball();
	//real
	upd->add(ball);
	upd->add(sta);
	//simu
	updSim->add(ball);
	updSim->add(sta);

	for(quint8 i=0; i<5; i++) {
		myTeam->push_back(new Robot(Robots::RobotIME2011(myTeam, i, i, BLUE)));
		//real
		comBTx->add(myTeam->last());
		upd->add(myTeam->last());
		//simu
		comBSim->add(myTeam->last());
		updSim->add(myTeam->last());

		enemyTeam->push_back(new Robot(Robots::RobotIME2011(enemyTeam, i, i, YELLOW)));
		//real
		comYTx->add(enemyTeam->last());
		upd->add(enemyTeam->last());
		//simu
		comYSim->add(enemyTeam->last());
		updSim->add(enemyTeam->last());
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

	skill1 = new Goto(this, myTeam->at(3), -1000, 0, 0, 500, true);//SteerToBall(this, myTeam->at(3), 0, 0);//
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
	
	cli->start();
	//connect(upd, SIGNAL(receiveUpdate()), filter, SLOT(step()));
}

Intelligence::~Intelligence()
{
	//TODO: delete stuff
}

void Intelligence::update() {
	mutex.lock();
	//QTime Tempo;
	//Tempo.start();

#ifdef SIMU
	updSim->step();
	updSim->apply();
#else
	upd->step();
	upd->apply();
#endif
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
	//attacker->setRobot(myTeam->at(0));
	//attacker->step();
	//attacker->setRobot(myTeam->at(1));
	//attacker->step();
	//attacker->setRobot(myTeam->at(2));
	//attacker->step();
	//attacker->setRobot(myTeam->at(3));
	//attacker->step();
	//attacker->setRobot(myTeam->at(4));
	//attacker->step();
	//machine->setRobot(myTeam->at(0));
	//machine->step();
	//machine->setRobot(myTeam->at(1));
	//machine->step();
	//machine->setRobot(myTeam->at(2));
	//machine->step();
	//machine->setRobot(myTeam->at(3));
	//machine->step();
	//machine->setRobot(myTeam->at(4));
	//machine->step();
	//((Skill*)machine->configuration().toList().at(1))->step();
	//player0->step();
	//player1->step();
	//player2->step();
	//player3->step();
	//player4->step();
	//play->step();
	//attacker->step();
	//skill1->step();
	//if(!skill3->busy()) cout << "DESOCUPADO!!!" << endl;
	//else cout << "OCUPADO!!!" << endl;
	//tactic->step();
	skill1->step();
	//tactic->step();

#ifdef SIMU
	comBSim->step();
	comBSim->send();
	comYSim->step();
	comYSim->send();
#else
	comBTx->step();
	comBTx->send();
#endif

	//int duracao=Tempo.elapsed();
	//printf("TEMPO: %i ms\n",duracao);
	mutex.unlock();
}
