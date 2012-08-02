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
#include "CommanderGrSim.h"
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

#include "config.h"

struct IntelligenceCli : public QThread
{
	Intelligence *intel;
	IntelligenceCli(Intelligence *intel) : QThread(intel), intel(intel) {}

	void run() {
		qreal x, y, s, kp, ki, kd;
		string command, sub;
		while(true) {
			cout << "> ";
			cin >> command;

			if(command == "quit" || command == "exit") {
				cout << "Bye!" << endl;
				intel->timer->stop();
				break;

			} else if(command[0] == 'z') {
				cin >> sub;
				intel->mutex.lock();
				if(sub[0] == 'n') {
					intel->mode = Intelligence::NONE;
					cout << "Switching to none." << endl;
				} else if(sub[0] == 's') {
					intel->mode = Intelligence::SKILL;
					cout << "Switching to skill." << endl;
				} else if(sub[0] == 't') {
					intel->mode = Intelligence::TACTIC;
					cout << "Switching to tactic." << endl;
				} else if(sub[0] == 'p') {
					intel->mode = Intelligence::PLAY;
					cout << "Switching to play." << endl;
				} else if(sub[0] == 'c') {
					intel->mode = Intelligence::CONTROLLER;
					cout << "Switching to controller." << endl;
				} else {
					cout << "Tipo nao reconhecido." << endl;
				}
				intel->mutex.unlock();

			} else if(command == "kick") {
				int i;
				cin >> i;
				cout << "Kick once" << endl;
				intel->mutex.lock();
				intel->team["us"]->at(i)->kick();
				intel->mutex.unlock();

			} else if(command == "dribble") {
				int i;
				cin >> i;
				cout << "Dribble once" << endl;
				intel->mutex.lock();
				intel->team["us"]->at(i)->dribble(1.0);
				intel->mutex.unlock();

			} else if(command == "sim") {
				cout << "Using simulation." << endl;
				intel->mutex.lock();
				intel->useSimulation = true;
				intel->resetPatterns();
				intel->mutex.unlock();

			} else if(command == "real") {
				cout << "Using real transmission/SSL." << endl;
				intel->mutex.lock();
				intel->useSimulation = false;
				intel->resetPatterns();
				intel->mutex.unlock();

			} else if(command == "sk") {
				cin >> s;
				cout << "setPowerK(" << s << ")" << endl;
				intel->mutex.lock();
				((SampledKick *)intel->skill["samk"])->setPowerK(s);
				intel->mutex.unlock();

			} else if(command[0] == 'm') {
				cin >> x >> y >> s;
				cout << "setSpeeds(" << x << "," << y << ")" << endl;
				cout << "setSpeedAngular(" << s << ")" << endl;
				intel->mutex.lock();
				((Move *)intel->skill["move"])->setSpeeds(x, y);
				((Move *)intel->skill["move"])->setSpeedAngular(s);
				intel->mutex.unlock();

			} else if(command[0] == 'p') {
				cin >> x >> y;
				cout << "setPoint(" << x << "," << y << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setPoint(x, y);
				intel->mutex.unlock();

			} else if(command[0] == 's') {
				cin >> s;
				cout << "setSpeed(" << s << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setSpeed(s);
				intel->mutex.unlock();

			} else if(command[0] == 'o') {
				cin >> s;
				cout << "setOrientarion(" << s << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setOrientation(s);
				intel->mutex.unlock();

			} else if(command[0] == 'c') {
				int i;
				cin >> i;
				cout << "controller robot " << i << endl;
				intel->mutex.lock();
				intel->tactic["controller"]->setRobot(intel->team["us"]->at(i));
				intel->mutex.unlock();

			} else if(command[0] == 'k') {
				cin >> kp >> ki >> kd;
				cout << "setPIDk(" << kp << "," << ki << "," << kd << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setPIDk(kp,ki,kd);
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
	cli(new IntelligenceCli(this)),
	useSimulation(true),
	mode(NONE)
{
	commander["blueSim"] = new CommanderSim(this);
    commander["blueGrSim"] = new CommanderGrSim(this);
	commander["blueTx"] = new CommanderTxOld(this);
	commander["yellowSim"] = new CommanderSim(this);
    commander["yellowGrSim"] = new CommanderGrSim(this);
	commander["yellowTx"] = new CommanderTxOld(this);

	updater["vision"] = new UpdaterVision(this);
	updater["visionSim"] = new UpdaterVision(this, 11002);
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

	for(quint8 i = 0; i < NPLAYERS; i++) {
        Robot* robot;
        robot = new Robot(Robots::RobotIME2011(team["us"], i, i, BLUE));
		team["us"]->push_back(robot);
		//real
		commander["blueTx"]->add(robot);
		updater["vision"]->add(robot);
		//simu
		commander["blueSim"]->add(robot);
        commander["blueGrSim"]->add(robot);
		updater["visionSim"]->add(robot);

        robot = new Robot(Robots::RobotIME2011(team["they"], i, i, YELLOW));
		team["they"]->push_back(robot);
		//real
		commander["yellowTx"]->add(robot);
		updater["vision"]->add(robot);
		//simu
		commander["yellowSim"]->add(robot);
        commander["yellowGrSim"]->add(robot);
		updater["visionSim"]->add(robot);
	}

#ifdef HAVE_WINDOWS
	tactic["controller"] = new Controller2(this, team["us"]->at(3), 1, 500); //controle no referencial do campo
	tactic["controller1"] = new Controller(this, team["us"]->at(3), 1, 1000); //controle no referencial do robo
#endif
	//skill["driveto"] = new DriveTo(this, team["us"]->at(1), -3.14/2., QPointF(0,0), 1000.);

	//skill["goto"] = new Goto(this, team["us"]->at(0), 1000, 0, 0, 3000, true);//SteerToBall(this, team["us"]->at(3), 0, 0);//
	skill["goto"] = new Goto(this, team["they"]->at(1), 1000, 0, 0, 3000, true);//SteerToBall(this, team["us"]->at(3), 0, 0);//
	skill["move"] = new Move(this, team["us"]->at(0), 0, 0, 0);
	skill["samk"] = new SampledKick(this, team["us"]->at(1), team["us"]->at(0), true, 0, 1, 3000, true);
	skill["samd"] = new SampledDribble(this, team["us"]->at(0), team["they"]->at(1), true, 1, 1, 1000);
	skill["loop"] = new Loops::Orbit(this, team["us"]->at(1), 0, 0, 1000, 3000, 1.0);
	skill["fac"] = new FollowAndCover(this, team["us"]->at(1), team["they"]->at(1), team["us"]->goal(), 1000, 3000);

	tactic["attacker"] = new Attacker(this, team["us"]->at(1), 3000);
	tactic["zickler43"] = new Zickler43(this, team["they"]->at(4), 3000, true);

	play["cbr"] = new Plays::CBR2011(this, team["they"], stage["main"]);
	play["cbr2"] = new Plays::CBR2011(this, team["us"], stage["main"]);
	play["retaliate"] = new Plays::AutoRetaliate(this, team["they"], stage["main"], 3000);
	tactic["attacker"] =  new AttackerMinMax2(this, team["us"]->at(1), 3000);
	//play["bgt"] = new Plays::BGT(this, team["us"], sta);
	play["minimax2"] = new Plays::Minmax2(this, team["us"], stage["main"]);
	//play["freekickem"] = new Plays::FreeKickThem(this, &team["us"], sta);

	tactic["gkpr"] = new Goalkeeper(this, team["they"]->at(0),1000);
	tactic["def"] = new Defender(this, team["they"]->at(1), 0, 1000);
	tactic["def2"] = new Defender(this, team["they"]->at(2), 0, 1000);
	tactic["def3"] = new Defender(this, team["they"]->at(3), 0, 1000);
	tactic["atk"] = new Attacker(this, team["they"]->at(4), 1000);
	
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(5.);//valor que tá no transmission da trunk para realTransmission //30.);//
	
	resetPatterns();
	cli->start();
}

void Intelligence::resetPatterns()
{
	if(useSimulation) {
		for(int i = 0; i < team["us"]->size(); i++)
			team["us"]->at(i)->setPatternId(i);
		for(int i = 0; i < team["they"]->size(); i++)
			team["they"]->at(i)->setPatternId(i);
	} else {
		team["us"]->at(0)->setPatternId(2);
		team["us"]->at(1)->setPatternId(3);
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
	}
}

void Intelligence::update()
{
#ifdef DEBUG_TIME
	QTime t;
	t.start();
#endif

	mutex.lock();

	if(useSimulation) {
		updater["visionSim"]->step();
		updater["visionSim"]->apply();
	} else {
		updater["vision"]->step();
		updater["vision"]->apply();
	}

	updater["referee"]->step();
	updater["referee"]->apply();

	///BEGIN STEPS
	switch(mode) {

	case PLAY:
		//play["cbr"]->step();
		//play["cbr2"]->step();
		//play["minimax2"]->step();
		if(!((QThread *)play["minimax2"])->isRunning())
			((QThread *)play["minimax2"])->start();
		play["minimax2"]->step();
		play["retaliate"]->step();
		break;

	case TACTIC:
		tactic["zickler43"]->step();
		tactic["attacker"]->step();
		break;

	case SKILL:
		skill["fac"]->step();
		skill["goto"]->step();
		break;

#ifdef HAVE_WINDOWS
	case CONTROLLER:
		tactic["controller"]->step();
#endif
	default:
		break;
	}
	///END STEPS

	if(useSimulation) {
#ifdef CONTROL_BLUE
        commander["blueGrSim"]->step();
		commander["blueSim"]->step();
        ((CommanderGrSim*)commander["blueGrSim"])->send();
		((CommanderSim*)commander["blueSim"])->send();
#endif
#ifdef CONTROL_YELLOW
        commander["yellowGrSim"]->step();
		commander["yellowSim"]->step();
        ((CommanderGrSim*)commander["yellowGrSim"])->send();
		((CommanderSim*)commander["yellowSim"])->send();
#endif
	} else {
#ifdef CONTROL_BLUE
		commander["blueTx"]->step();
		((CommanderTxOld*)commander["blueTx"])->send();
#endif
#ifdef CONTROL_YELLOW
		//commander["yellowTx"]->step();
		//((CommanderTxOld*)commander["yellowTx"])->send();
#endif
	}

	mutex.unlock();

#ifdef DEBUG_TIME
	cout << "TEMPO: " << t.elapsed() << "ms" << endl;
#endif
}

Intelligence::~Intelligence()
{
	team.clear();
	robot.clear();
	stage.clear();
	play.clear();
	tactic.clear();
	skill.clear();
	updater.clear();
	commander.clear();
	delete timer, cli;
}
