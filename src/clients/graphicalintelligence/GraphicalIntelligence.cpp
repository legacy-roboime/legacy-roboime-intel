#include "GraphicalIntelligence.h"
#include <QtCore>

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

#include "config.h"

struct IntelligenceCli : public QThread
{
	GraphicalIntelligence *intel;
	IntelligenceCli(GraphicalIntelligence *intel) : QThread(intel), intel(intel) {}

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
					intel->mode = GraphicalIntelligence::NONE;
					cout << "Switching to none." << endl;
				} else if(sub[0] == 's') {
					intel->mode = GraphicalIntelligence::SKILL;
					cout << "Switching to skill." << endl;
				} else if(sub[0] == 't') {
					intel->mode = GraphicalIntelligence::TACTIC;
					cout << "Switching to tactic." << endl;
				} else if(sub[0] == 'p') {
					intel->mode = GraphicalIntelligence::PLAY;
					cout << "Switching to play." << endl;
				} else if(sub[0] == 'c') {
					intel->mode = GraphicalIntelligence::CONTROLLER;
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
				intel->mutex.unlock();

			} else if(command[0] == 'k') {
				cin >> kp >> ki >> kd;
				cout << "setPIDk(" << kp << "," << ki << "," << kd << ")" << endl;
				intel->mutex.lock();
				((Goto *)intel->skill["goto"])->setPIDk(kp,ki,kd);
				intel->mutex.unlock();

			}else {
				cout << "Comando nao reconhecido." << endl;

			}
			cin.clear();
			cout.clear();
		}
	}
};

GraphicalIntelligence::GraphicalIntelligence(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	cli(new IntelligenceCli(this)),
	useSimulation(true),
	mode(NONE)
{
	// Popular nomes dos estados do Juiz
	refereeFull['H'] = tr("Halt");
	refereeFull['S'] = tr("Stop");
	refereeFull[' '] = tr("Ready");
	refereeFull['s'] = tr("Start");
	refereeFull['1'] = tr("Begin first half");
	refereeFull['h'] = tr("Begin half time");
	refereeFull['2'] = tr("Begin second half");
	refereeFull['o'] = tr("Begin overtime half 1");
	refereeFull['O'] = tr("Begin overtime half 2");
	refereeFull['a'] = tr("Begin penalty shootout");
	refereeFull['k'] = tr("Kick off - Yellow Team");
	refereeFull['K'] = tr("Kick off - Blue Team");
	refereeFull['p'] = tr("Penalty - Yellow Team");
	refereeFull['P'] = tr("Penalty - Blue Team");
	refereeFull['f'] = tr("Direct Free kick - Yellow Team");
	refereeFull['F'] = tr("Direct Free kick - Blue Team");
	refereeFull['i'] = tr("Indirect Free kick - Yellow Team");
	refereeFull['I'] = tr("Indirect Free kick - Blue Team");
	refereeFull['t'] = tr("Timeout - Yellow Team");
	refereeFull['T'] = tr("Timeout - Blue Team");
	refereeFull['z'] = tr("Timeout end - Yellow Team");
	refereeFull['Z'] = tr("Timeout end - Blue Team");
	refereeFull['g'] = tr("Goal scored - Yellow Team");
	refereeFull['G'] = tr("Goal scored - Blue Team");
	refereeFull['d'] = tr("decrease Goal score - Yellow Team");
	refereeFull['D'] = tr("decrease Goal score - Blue Team");
	refereeFull['y'] = tr("Yellow Card - Yellow Team");
	refereeFull['Y'] = tr("Yellow Card - Blue Team");
	refereeFull['r'] = tr("Red Card - Yellow Team");
	refereeFull['R'] = tr("Red Card - Blue Team");
	refereeFull['c'] = tr("Cancel");
	refereeFull['H'] = tr("Halt");

	ui.setupUi(this);

	commander["blueSim"] = new CommanderSim(this);
	commander["blueTx"] = new CommanderTxOld(this);
	commander["yellowSim"] = new CommanderSim(this);
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
		team["us"]->push_back(new Robot(Robots::RoboIME2012(team["us"], i, i, BLUE)));
		//real
		commander["blueTx"]->add(team["us"]->last());
		updater["vision"]->add(team["us"]->last());
		//simu
		commander["blueSim"]->add(team["us"]->last());
		updater["visionSim"]->add(team["us"]->last());

		team["they"]->push_back(new Robot(Robots::RoboIME2012(team["they"], i, i, YELLOW)));
		//real
		commander["yellowTx"]->add(team["they"]->last());
		updater["vision"]->add(team["they"]->last());
		//simu
		commander["yellowSim"]->add(team["they"]->last());
		updater["visionSim"]->add(team["they"]->last());
	}

	skill["driveto"] = new DriveTo(this, team["us"]->at(1), 100, 0.174, (M_PI/4)*3., Point(0,0), 1000, (M_PI/4)*3.);
	skill["drivetoObj"] = new DriveToObject(this, team["us"]->at(1), team["they"]->at(1), -500, stage["main"]->ball());
	skill["steer"] = new SteerToBall(this, team["us"]->at(3), 0, 0);
	skill["goto"] = new Goto(this, team["us"]->at(3), 0, 0, 0, 500, true);
	skill["move"] = new Move(this, team["us"]->at(0), 0, 0, 0);
	skill["samk"] = new SampledKick(this, team["us"]->at(1), team["us"]->at(0), true, 0, 1, 3000, false);
	skill["samd"] = new SampledDribble(this, team["us"]->at(0), team["they"]->at(1), true, 1, 1, 1000);
	skill["loop"] = new Loops::Orbit(this, team["us"]->at(1), 0, 0, 1000, 3000, 1.0);
	skill["fac"] = new FollowAndCover(this, team["us"]->at(1), team["they"]->at(1), team["us"]->goal(), 1000, 3000);

	play["cbr"] = new Plays::CBR2011(this, team["they"], stage["main"]);
	play["cbr2"] = new Plays::CBR2011(this, team["us"], stage["main"]);
	play["retaliateU"] = new Plays::AutoRetaliate(this, team["us"], stage["main"], team["us"]->at(2), 3000);
	play["retaliateT"] = new Plays::AutoRetaliate(this, team["they"], stage["main"], team["they"]->at(0), 3000);
#ifdef HAVE_WINDOWS
	play["bgt"] = new Plays::BGT(this, team["us"], stage["main"]);
#endif
	play["minimax2"] = new Plays::Minmax2(this, team["us"], stage["main"]);
	play["freekickem"] = new Plays::FreeKickThem(this, team["us"], stage["main"]);
	play["refereeU"] = new Plays::ObeyReferee(this, play["retaliateU"]/*play["minimax2"]*/, team["us"]->at(2), team["us"]->at(1));
	play["refereeT"] = new Plays::ObeyReferee(this, play["retaliateT"], team["they"]->at(0), team["they"]->at(1));
	play["stoprefT"] = new Plays::StopReferee(this, team["they"], stage["main"], team["they"]->at(0));

	tactic["attacker"] =  new AttackerMinMax2(this, team["us"]->at(1), 3000);
#ifdef HAVE_WINDOWS
	tactic["controller"] = new Controller2(this, team["us"]->at(0), 1, 3000); //controle no referencial do robo
	tactic["controller1"] = new Controller(this, team["us"]->at(0), 1, 3000); //controle no referencial do campo
#endif
	tactic["attacker"] = new Attacker(this, team["us"]->at(1), 3000);
	tactic["zickler43"] = new Zickler43(this, team["us"]->at(4), 3000, true);
	tactic["gkpr"] = new Goalkeeper(this, team["us"]->at(0),3000);
	tactic["def"] = new Defender(this, team["us"]->at(3), team["they"]->at(0), 500, 3000);
	tactic["def2"] = new Defender(this, team["they"]->at(2), team["us"]->at(2), 500, 1000);
	tactic["def3"] = new Defender(this, team["they"]->at(3), team["us"]->at(3), 500, 1000);
	tactic["atk"] = new Attacker(this, team["they"]->at(4), 1000);

	ui.stageView->setStage(stage["main"]);
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10.);

	resetPatterns();
	cli->start();
}

GraphicalIntelligence::~GraphicalIntelligence()
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

void GraphicalIntelligence::resetPatterns()
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

void GraphicalIntelligence::update()
{
	if (ui.chkUpdate->isChecked()) {
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
			//if(!((QThread *)play["minimax2"])->isRunning())
			//	((QThread *)play["minimax2"])->start();
			//play["minimax2"]->step();
			play["refereeU"]->step();
			play["refereeT"]->step();
			//play["stoprefT"]->step();
			//play["retaliateT"]->step();
			//play["retaliateU"]->step();
			//tactic["zickler43"]->step();
			//play["retaliateT"]->step();
			break;

		case TACTIC:
			tactic["zickler43"]->step();
			//tactic["gkpr"]->step();
			//tactic["def"]->step();
			break;

		case SKILL:
			//skill["fac"]->step();
			skill["goto"]->step();
			skill["samk"]->step();
			//((Goto*)skill["goto"])->setAllowDefenseArea();
			//skill["drivetoObj"]->step();
			break;

		case CONTROLLER:
			tactic["controller"]->step();
		default:
			break;
		}
		///END STEPS



		if(useSimulation) {
#ifdef CONTROL_BLUE
			commander["blueSim"]->step();
			((CommanderSim*)commander["blueSim"])->send();
#endif
#ifdef CONTROL_YELLOW
			commander["yellowSim"]->step();
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

	updateValues();
}

void GraphicalIntelligence::updateValues()
{
	QString textoJuiz = refereeFull.value(stage["main"]->getCmdReferee());
	if (textoJuiz.isEmpty())
		textoJuiz = QString(QChar(stage["main"]->getCmdReferee()));
	ui.txtJuiz->setText(textoJuiz);

	/// Juiz
	//ui.txtJuiz->setText(QChar(stage["main"]->getCmdReferee()));

	ui.txtTempoJogo->setText(QString::number(stage["main"]->getTimeLeft()));

	/// Placar
	//stage["main"]->blueTeam()->goals();
	//ui.txtPlacarAzul->setText(QString::number(stage["main"]->blueTeam()->goals()));
	//ui.txtPlacarAmarelo->setText(QString::number(stage["main"]->yellowTeam()->goals()));
	UpdaterReferee* updR = qobject_cast<UpdaterReferee*>(updater["vision"]);



	/// Timeouts
	//ui.txtNrTimeouts->setText(QString::number(stage["main"]->));
	//ui.txtTempoTimeouts->setText(QString::number(stage["main"]->));

	ui.txtStageLenght->setText(QString::number(stage["main"]->fieldLength()));
	ui.txtStageWidth->setText(QString::number(stage["main"]->fieldWidth()));

	ui.stageView->redraw();
}


void GraphicalIntelligence::on_btnIntStart_clicked()
{
	timer->start();
}

void GraphicalIntelligence::on_btnIntStop_clicked()
{
	timer->stop();
}
