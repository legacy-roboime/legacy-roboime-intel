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

			} else if(command == "kicker") {
				int i;
				float working;
				cin >> i >> working;
				if(i>=intel->team["us"]->size())
					cout << "ID INVALIDO" << endl;
				else{
					cout << "SetWorkingKicker(" << working << ")" << endl;
					intel->mutex.lock();
					intel->team["us"]->at(i)->kicker().setWorking(working);
					intel->mutex.unlock();
				}

			} else if(command == "dribbler") {
				int i;
				float working;
				cin >> i >> working;
				if(i>=intel->team["us"]->size())
					cout << "ID INVALIDO" << endl;
				else{
					cout << "SetWorkingDribbler(" << working << ")" << endl;
					intel->mutex.lock();
					intel->team["us"]->at(i)->dribbler().setWorking(working);
					intel->mutex.unlock();
				}

			} 
			else if(command == "kick") {
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

			}
			else if(command == "side") {
				string side;
				cin >> side;
				cout << "setSide(" << side << ")" << endl;
				intel->mutex.lock();
				if(side.compare("left") == 0){ //left
					if(intel->team["us"]->color() == BLUE)
						intel->stage["main"]->setIsLeftSideBlueGoal(true);
					else
						intel->stage["main"]->setIsLeftSideBlueGoal(false);
				}
				else{
					if(intel->team["us"]->color() == BLUE)
						intel->stage["main"]->setIsLeftSideBlueGoal(false);
					else
						intel->stage["main"]->setIsLeftSideBlueGoal(true);
				}
				intel->mutex.unlock();

			}else if(command[0] == 's') {
				cin >> s;
				cout << "setSpeed(" << s << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setSpeed(s);
				intel->tactic["zickler43"]->setSpeed(s);
				intel->tactic["controller"]->setSpeed(s);
				intel->tactic["controller1"]->setSpeed(s);
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
				intel->tactic["controller1"]->setRobot(intel->team["us"]->at(i));
				//intel->tactic["zickler43"]->setRobot(intel->team["us"]->at(i));
				intel->skill["goto"]->setRobot(intel->team["us"]->at(i));
				intel->mutex.unlock();

			} else if(command[0] == 'k') {
				cin >> kp >> ki >> kd;
				cout << "setPIDk(" << kp << "," << ki << "," << kd << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setPIDk(kp,ki,kd);
				intel->mutex.unlock();

            } else if(command[0] == 'g') {
                int i;
                cin >> i;
                if(i>=intel->team["us"]->size())
                    cout << "ID INVALIDO" << endl;
                else{
                    cout << "Goalkeeper(" << i << ")" << endl;
                    intel->mutex.lock();
                    ((ObeyReferee *)intel->play["refereeU"])->setGoalkeeper( intel->team["us"]->at(i) );
					((AutoRetaliate *)intel->play["retaliateU"])->setGoalkeeper( intel->team["us"]->at(i) );
                    intel->mutex.unlock();
                }

			}
			else {
				cout << "Comando nao reconhecido." << endl;

			}
			cin.clear();
			cout.clear();
		}
	}
};

Intelligence::Intelligence(QObject *parent)
	: QObject(parent),
	mode(NONE),
	useSimulation(true),
	cli(new IntelligenceCli(this))
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
        robot = new Robot(Robots::RoboIME2012(team["us"], i, i, BLUE));
		team["us"]->push_back(robot);
		//real
		commander["blueTx"]->add(robot);
		updater["vision"]->add(robot);
		//simu
		commander["blueSim"]->add(robot);
        commander["blueGrSim"]->add(robot);
		updater["visionSim"]->add(robot);

        robot = new Robot(Robots::RoboIME2012(team["they"], i, i, YELLOW));
		team["they"]->push_back(robot);
		//real
		commander["yellowTx"]->add(robot);
		updater["vision"]->add(robot);
		//simu
		commander["yellowSim"]->add(robot);
        commander["yellowGrSim"]->add(robot);
		updater["visionSim"]->add(robot);
	}

	Robot* gkUs = team["us"]->at(2);
	Robot* gkThem = team["they"]->at(0);
	Robot* pKickerUs = team["us"]->at(1);
	Robot* pKickerThem = team["they"]->at(1);
	
	skill["driveto"] = new DriveTo(this, team["us"]->at(1), 100, 0.174, (M_PI/4)*3., Point(0,0), 1000, (M_PI/4)*3.);
	skill["drivetoObj"] = new DriveToObject(this, team["us"]->at(1), team["they"]->at(1), -500, stage["main"]->ball());
	skill["drivetoBall"] = new DriveToBall(this, team["us"]->at(0), team["they"]->at(0));
	skill["steer"] = new SteerToBall(this, team["us"]->at(3), 0, 0);
	skill["goto"] = new Goto(this, team["us"]->at(2), 0, 0, 0, 3000, true);
	skill["gotoa"] = new GotoAvoid(this, team["us"]->at(0), new Point(0, 0), stage["main"]->ball(), stage["main"]->ball()->radius() + team["us"]->at(0)->body().cut(), 3000);
	skill["move"] = new Move(this, team["us"]->at(0), 0, 0, 0);
	skill["samk"] = new SampledKick(this, team["us"]->at(2), team["us"]->at(2), true, 0, 1, 3000, false);
	skill["samd"] = new SampledDribble(this, team["us"]->at(1), team["they"]->at(1), true, 1, 1, 3000);
	skill["loop"] = new Loops::Orbit(this, team["us"]->at(1), 0, 0, 1000, 3000, 1.0);
	skill["fac"] = new FollowAndCover(this, team["us"]->at(1), team["they"]->at(1), team["us"]->goal(), 1000, 3000);

	play["cbr"] = new Plays::CBR2011(this, team["they"], stage["main"]);
	play["cbr2"] = new Plays::CBR2011(this, team["us"], stage["main"]);
#ifdef HAVE_WINDOWS
	play["bgt"] = new Plays::BGT(this, team["us"], stage["main"]);
#endif
	play["freekickem"] = new Plays::FreeKickThem(this, team["us"], stage["main"]);
	play["retaliateU"] = new Plays::AutoRetaliate(this, team["us"], stage["main"], gkUs, 3000);
	play["retaliateT"] = new Plays::AutoRetaliate(this, team["they"], stage["main"], gkThem, 6000);
	play["refereeU"] = new Plays::ObeyReferee(this, play["retaliateU"]/*play["minimax2"]*/, gkUs, pKickerUs);
	play["refereeT"] = new Plays::ObeyReferee(this, play["retaliateT"], gkThem, pKickerThem);
	play["stoprefT"] = new Plays::StopReferee(this, team["they"], stage["main"], team["they"]->at(0));

#ifdef HAVE_WINDOWS
	tactic["controller"] = new Controller2(this, team["us"]->at(0), 1, 3000); //controle no referencial do robo
	tactic["controller1"] = new Controller(this, team["us"]->at(0), 1, 3000); //controle no referencial do campo
#endif
	tactic["attacker"] = new Attacker(this, team["us"]->at(1), 3000);
	tactic["zickler43U"] = new Zickler43(this, team["us"]->at(2), 3000, true);
	tactic["goalkeeperT"] = new Goalkeeper(this, team["they"]->at(0),3000);
	tactic["gkpr"] = new Goalkeeper(this, team["us"]->at(0),3000);
	tactic["def"] = new Defender(this, team["us"]->at(3), team["they"]->at(0), team["us"]->goal(), 500, 3000);
	tactic["def2"] = new Defender(this, team["they"]->at(2), team["us"]->at(2), team["us"]->goal(), 500, 1000);
	tactic["def3"] = new Defender(this, team["they"]->at(3), team["us"]->at(3), team["us"]->goal(), 500, 1000);
	tactic["atk"] = new Attacker(this, team["they"]->at(4), 1000);
	tactic["bl"] = new Blocker(this, team["they"]->at(4), 0, 6000);
	
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(5.);//valor que t� no transmission da trunk para realTransmission //30.);//
	
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
		team["us"]->at(0)->setPatternId(0);
		team["us"]->at(1)->setPatternId(1);
		team["us"]->at(2)->setPatternId(2);
		team["us"]->at(3)->setPatternId(3);
		team["us"]->at(4)->setPatternId(4);
		team["us"]->at(5)->setPatternId(5);

		team["they"]->at(0)->setPatternId(0);
		team["they"]->at(1)->setPatternId(1);
		team["they"]->at(2)->setPatternId(2);
		team["they"]->at(3)->setPatternId(3);
		team["they"]->at(4)->setPatternId(4);
		team["they"]->at(5)->setPatternId(5);
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
	if(!((UpdaterVision*)updater["vision"])->timeout()) switch(mode) {
	case PLAY:
		//play["cbr"]->step();
		//play["cbr2"]->step();
		//if(!((QThread *)play["minimax2"])->isRunning())
		//	((QThread *)play["minimax2"])->sntart();
		//play["minimax2"]->step();
		//play["bgt"]->step();
		play["refereeU"]->step();
		//play["refereeT"]->step();
		//play["stoprefT"]->step();
		//play["retaliateT"]->step();
		//play["retaliateU"]->step();
		//tactic["zickler43"]->step();
		//play["retaliateT"]->step();
		break;

	case TACTIC:
		//tactic["attackerM"]->step();
		tactic["zickler43U"]->step();
		tactic["goalkeeperT"]->step();
		//tactic["bl"]->step();
		//tactic["gkpr"]->step();
		//tactic["def"]->step();
		break;

	case SKILL:
		//skill["fac"]->step();
		//skill["drivetoBall"]->step();
		//skill["samd"]->step();
		//cout << skill["gotoa"]->busy() << endl;
		skill["goto"]->step();
		//skill["samk"]->step();
		//((Goto*)skill["goto"])->setAllowDefenseArea();
		//skill["drivetoObj"]->step();
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
		// WARNING: CommanderSim resets the robots' command, while CommanderGrSim doesn't. As such, when using both, always dispatch GrSim commands first!
		commander["blueGrSim"]->step();
		commander["blueSim"]->step();
		((CommanderGrSim*)commander["blueGrSim"])->send();
		((CommanderSim*)commander["blueSim"])->send();
#endif
#ifdef CONTROL_YELLOW
		// WARNING: CommanderSim resets the robots' command, while CommanderGrSim doesn't. As such, when using both, always dispatch GrSim commands first!
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
	delete timer;//, cli;//why?????
}
