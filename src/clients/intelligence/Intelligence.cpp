#include "Intelligence.h"

#include <QThread>
#include <QCoreApplication>
#include <iostream>

#include "Robot.h"
#include "Goal.h"
#include "Robots.h"
#include "Skills.h"
#include "Tactics.h"
#include "Plays.h"
#include "CommanderTx.h"
#include "CommanderTxOld.h"
#include "CommanderSim.h"
#include "UpdaterVision.h"
#include "UpdaterReferee.h"
#include "Ball.h"
#include "KalmanFilters.h"
#include "UpdateStageGeometry.h"
#include "UpdaterReferee.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;
using namespace LibIntelligence::Tactics;
using namespace LibIntelligence::Plays;

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

			if(command[0] == 'p') {
				cin >> x >> y;
				cout << "skill1->SetPoint(" << x << "," << y << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setPoint(x, y);
				intel->mutex.unlock();

			} else if(command[0] == 's') {
				cin >> s;
				cout << "skill1->SetSpeed(" << s << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setSpeed(s);
				intel->mutex.unlock();

			} else {
				cout << "Comando nao reconhecido." << endl;

			}
			cin.clear();
			cout.clear();
		}
	}
};

Intelligence::Intelligence(QObject *parent)
	: QObject(parent),
	cli(new IntelligenceCli(this))
{
	commander["blueSim"] = new CommanderSim(this);
	commander["blueTx"] = new CommanderTxOld(this);
	commander["yellowSim"] = new CommanderSim(this);
	commander["yellowTx"] = new CommanderTxOld(this);

	updater["vision"] = new UpdaterVision(this);
	updater["visionSim"] = new UpdaterVision(this, "224.5.23.2", 11002);
	updater["referee"] = new UpdaterReferee(this);

	stage["main"] = new Stage();
	team["us"] = stage["main"]->blueTeam();
	team["they"] = stage["main"]->yellowTeam();

	updater["referee"]->add(stage["main"]);

	//real
	updater["vision"]->add(stage["main"]->ball());
	updater["vision"]->add(stage["main"]);

	//simu
	updater["visionSim"]->add(stage["main"]->ball());
	updater["visionSim"]->add(stage["main"]);

	for(quint8 i=0; i<5; i++) {
		team["us"]->push_back(new Robot(Robots::RobotIME2011(team["us"], i, i, BLUE)));
		//real
		commander["blueTx"]->add(team["us"]->last());
		updater["vision"]->add(team["us"]->last());
		//simu
		commander["blueSim"]->add(team["us"]->last());
		updater["visionSim"]->add(team["us"]->last());

		team["they"]->push_back(new Robot(Robots::RobotIME2011(team["they"], i, i, YELLOW)));
		//real
		commander["yellowTx"]->add(team["they"]->last());
		updater["vision"]->add(team["they"]->last());
		//simu
		commander["yellowSim"]->add(team["they"]->last());
		updater["visionSim"]->add(team["they"]->last());
	}

	//team["us"]->at(0)->setPatternId(1);
	//team["us"]->at(1)->setPatternId(1);
	//team["us"]->at(2)->setPatternId(1);
	//team["us"]->at(3)->setPatternId(1);
	//team["us"]->at(3)->setPatternId(1);
	//team["they"][0]->setPatternId(4);

	//set the kicker if it is not working
	//team["us"][0]->kicker().setNotWorking();
	//team["us"][1]->kicker().setNotWorking();
	//team["us"][2]->kicker().setNotWorking();
	//team["us"][3]->kicker().setNotWorking();
	//team["us"][4]->kicker().setNotWorking();

	tactic["controller"] = new Controller(this, team["us"]->at(3), 1, 500); //controle no referencial do campo
	tactic["controller2"] = new Tactics::Controller2(this, team["us"]->at(3), 1, 1000); //controle no referencial do robo
	//skill["driveto"] = new DriveTo(this, team["us"]->at(1), -3.14/2., QPointF(0,0), 1000.);

	skill["goto"] = new Goto(this, team["us"]->at(3), -1000, 0, 0, 500, true);//SteerToBall(this, team["us"]->at(3), 0, 0);//
	skill["sampledKick"] = new SampledKick(this, team["us"]->at(1), team["they"]->goal(), true, 0, 1, 500, false);
	skill["sampledDribble"] = new SampledDribble(this, team["us"]->at(1), team["they"]->at(1), true, 1, 1, 1000);

	tactic["attacker"] = new Attacker(this, team["us"]->at(1), 1000);
	tactic["zickler43"] = new Zickler43(this, team["us"]->at(3), 1000, true);

	play["cbr2011"] = new Plays::CBR2011(this, team["they"], stage["main"]);
	tactic["attacker"] =  new AttackerMinMax2(this, team["us"]->at(1), 3000);
	//play["bgt"] = new Plays::BGT(this, team["us"], sta);
	play["minimax2"] = new Plays::Minmax2(this, team["us"], stage["main"]);
	//play["freekickem"] = new Plays::FreeKickThem(this, &team["us"], sta);

	tactic["gkpr"] = new Goalkeeper(this, team["they"]->at(0),1000);
	tactic["def"] = new Defender(this, team["they"]->at(1), 0, 1000);
	tactic["def2"] = new Defender(this, team["they"]->at(2), 0, 1000);
	tactic["def3"] = new Defender(this, team["they"]->at(3), 0, 1000);
	tactic["atk"] = new Attacker(this, team["they"]->at(4), 1000);
	
#ifndef SOCCER_DEBUG
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10.);//valor que tá no transmission da trunk para realTransmission //30.);//
#endif
	
	cli->start();
}

Intelligence::~Intelligence()
{
	updater.clear();
}

void Intelligence::update() {
	mutex.lock();
#ifdef DEBUG_TIME
	QTime t;
	t.start();
#endif

#ifdef SIMU
	updater["visionSim"]->step();
	updater["visionSim"]->apply();
#else
	updater["vision"]->step();
	updater["vision"]->apply();
#endif
	updater["referee"]->step();
	updater["referee"]->apply();

	///BEGIN STEPS
	play["cbr2011"]->step();
	play["minimax2"]->step();

	///END STEPS

#ifdef SIMU
	commander["blueSim"]->step();
	((CommanderSim*)commander["blueSim"])->send();
	commander["yellowSim"]->step();
	((CommanderSim*)commander["yellowSim"])->send();
#else
	commander["blue"]->step();
	((CommanderTxOld*)commander["blue"])->send();
#endif

#ifdef DEBUG_TIME
	cout << "TEMPO: " << t.elapsed() << "ms" << endl;
#endif
	mutex.unlock();
}
