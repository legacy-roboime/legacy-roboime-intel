#include "GraphicalIntelligence.h"
#include "alterstatevars.h"

#include <QtCore>
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
#include "winmain.h"

GraphicalIntelligence::GraphicalIntelligence(QWidget *parent)
	: QMainWindow(parent),
	useSimulation(true),
    mode(PLAY),
    alterStateVarsWindow(new AlterStateVars(this)),
    //current_play_us(NULL),
    //current_play_them(NULL),
    current_tactic_us(NULL),
    current_tactic_them(NULL)

{
    load_configs();
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
    commander["blueGrSim"] = new CommanderGrSim(this);
	commander["blueTx"] = new CommanderTxOld(this);
    commander["yellowSim"] = new CommanderSim(this);
    commander["yellowGrSim"] = new CommanderGrSim(this);
	commander["yellowTx"] = new CommanderTxOld(this);

	updater["vision"] = new UpdaterVision(this);
	//((UpdaterVision *)updater["vision"])->setWantedCam(0);
	updater["visionSim"] = new UpdaterVision(this, 11002);

	updater["referee"] = new UpdaterReferee(this);
	updater["refereeSim"] = new UpdaterReferee(this, "224.5.23.1", 10101);

	stage["main"] = new Stage();
	
	team["us"] = stage["main"]->yellowTeam();
	team["they"] = stage["main"]->blueTeam();
	
	
	/*team["us"] = stage["main"]->blueTeam();
	team["they"] = stage["main"]->yellowTeam();*/
	

	updater["referee"]->add(stage["main"]);
	updater["refereeSim"]->add(stage["main"]);

	//real
	updater["vision"]->add(stage["main"]->ball());
	updater["vision"]->add(stage["main"]);

	//simu
	updater["visionSim"]->add(stage["main"]->ball());
	updater["visionSim"]->add(stage["main"]);

	for(quint8 i = 0; i < NPLAYERS; i++) {
        team["us"]->push_back(new Robot(Robots::RoboIME2013(team["us"], i, i,YELLOW)));
		//real
        commander["blueTx"]->add(team["us"]->last());
		updater["vision"]->add(team["us"]->last());
		//simu
		commander["blueSim"]->add(team["us"]->last());
        commander["blueGrSim"]->add(team["us"]->last());
		updater["visionSim"]->add(team["us"]->last());
	}
	// FIXME: THE GAMBIARRATION
	for(quint8 i = 0; i < 9; i++) {
		team["they"]->push_back(new Robot(Robots::RoboIME2013(team["they"], i, i,BLUE)));
		//real
		commander["yellowTx"]->add(team["they"]->last());
		updater["vision"]->add(team["they"]->last());
		//simu
		commander["yellowSim"]->add(team["they"]->last());
        commander["yellowGrSim"]->add(team["they"]->last());
		updater["visionSim"]->add(team["they"]->last());
	}
	Robot* gkUs = team["us"]->at(4);
	Robot* gkThem = team["they"]->at(0);
	Robot* pKickerUs = team["us"]->at(4);
	Robot* pKickerThem = team["they"]->at(1);

	skill["driveto"] = new DriveTo(this, team["us"]->at(1), 100, 0.174, (M_PI/4)*3., Point(0,0), 1000, (M_PI/4)*3.);
	skill["drivetoObj"] = new DriveToObject(this, team["us"]->at(1), team["they"]->at(1), -500, stage["main"]->ball());
	skill["drivetoBall"] = new DriveToBall(this, team["us"]->at(0), team["they"]->at(0));
	skill["steer"] = new SteerToBall(this, team["us"]->at(3), 0, 0);
	skill["goto"] = new Goto(this, team["us"]->at(3), 0, 0, 0, 500, true);
	skill["gotoa"] = new GotoAvoid(this, team["us"]->at(0), new Point(0, 0), stage["main"]->ball(), stage["main"]->ball()->radius() + team["us"]->at(0)->body().cut(), 3000);
	skill["move"] = new Move(this, team["us"]->at(0), 0, 0, 0);
	skill["samk"] = new SampledKick(this, team["us"]->at(2), team["us"]->at(3), true, 0, 1, 3000, false);
	skill["samd"] = new SampledDribble(this, team["us"]->at(1), team["they"]->at(1), true, 1, 1, 3000);
	skill["loop"] = new Loops::Orbit(this, team["us"]->at(1), 0, 0, 1000, 3000, 1.0);
	skill["fac"] = new FollowAndCover(this, team["us"]->at(1), team["they"]->at(1), team["us"]->goal(), 1000, 3000);

    play["cbr2"] = new Plays::CBR2011(this, team["they"], stage["main"]);
    play["cbr"] = new Plays::CBR2011(this, team["us"], stage["main"]);
	play["retaliateU"] = new Plays::AutoRetaliate(this, team["us"], stage["main"], gkUs, 3000);
	play["retaliateT"] = new Plays::AutoRetaliate(this, team["they"], stage["main"], gkThem, 3000);
	//play["minimax2"] = new Plays::Minmax2(this, team["us"], stage["main"]);
	play["freekickem"] = new Plays::FreeKickThem(this, team["us"], stage["main"]);
	play["refereeU"] = new Plays::ObeyReferee(this, play["retaliateU"]/*play["minimax2"]*/, gkUs, pKickerUs);
	play["refereeT"] = new Plays::ObeyReferee(this, play["retaliateT"], gkThem, pKickerThem);
	play["stoprefT"] = new Plays::StopReferee(this, team["they"], stage["main"], team["they"]->at(0));
    play["haltU"] = new Plays::Halt(this, team["us"], team["us"]->stage());
    current_play_us = play["haltU"];
    play["haltT"] = new Plays::Halt(this, team["they"], team["they"]->stage());
    current_play_them = play["haltT"];
	//tactic["attackerM"] =  new AttackerMinMax2(this, team["us"]->at(1), team["they"]->at(1), team["they"]->at(1), team["they"]->at(1), 3000, 3000);
#ifdef HAVE_WINDOWS
    tactic["controller_b1"] = new Controller2(this, team["us"]->at(0), 1, 3000); //controle no referencial do robo
    tactic["controller_b2"] = new Controller2(this, team["us"]->at(1), 1, 3000); //controle no referencial do robo
    tactic["controller_b3"] = new Controller2(this, team["us"]->at(2), 1, 3000); //controle no referencial do robo
    tactic["controller_b4"] = new Controller2(this, team["us"]->at(3), 1, 3000); //controle no referencial do robo
    tactic["controller_b5"] = new Controller2(this, team["us"]->at(4), 1, 3000); //controle no referencial do robo
    tactic["controller_b6"] = new Controller2(this, team["us"]->at(5), 1, 3000); //controle no referencial do robo

    current_tactic_us = tactic["controller_b1"];

	tactic["controller_b1a"] = new Controller(this, team["us"]->at(0), 1, 3000); //controle no referencial do robo
    tactic["controller_b2a"] = new Controller(this, team["us"]->at(1), 1, 3000); //controle no referencial do robo
    tactic["controller_b3a"] = new Controller(this, team["us"]->at(2), 1, 3000); //controle no referencial do robo
    tactic["controller_b4a"] = new Controller(this, team["us"]->at(3), 1, 3000); //controle no referencial do campo
    tactic["controller_b5a"] = new Controller(this, team["us"]->at(4), 1, 3000); //controle no referencial do campo
    tactic["controller_b6a"] = new Controller(this, team["us"]->at(5), 1, 3000); //controle no referencial do campo

    tactic["controller_y1"] = new Controller2(this, team["they"]->at(0), 1, 3000); //controle no referencial do robo
    tactic["controller_y2"] = new Controller2(this, team["they"]->at(1), 1, 3000); //controle no referencial do robo
    tactic["controller_y3"] = new Controller2(this, team["they"]->at(2), 1, 3000); //controle no referencial do robo
    tactic["controller_y4"] = new Controller2(this, team["they"]->at(3), 1, 3000); //controle no referencial do robo
    tactic["controller_y5"] = new Controller2(this, team["they"]->at(4), 1, 3000); //controle no referencial do robo
    tactic["controller_y6"] = new Controller2(this, team["they"]->at(5), 1, 3000); //controle no referencial do robo
    
    current_tactic_them = tactic["controller_y1"];

	tactic["controller_y1a"] = new Controller(this, team["they"]->at(0), 1, 3000); //controle no referencial do robo
    tactic["controller_y2a"] = new Controller(this, team["they"]->at(1), 1, 3000); //controle no referencial do robo
    tactic["controller_y3a"] = new Controller(this, team["they"]->at(2), 1, 3000); //controle no referencial do robo
    tactic["controller_y4a"] = new Controller(this, team["they"]->at(3), 1, 3000); //controle no referencial do ca
	tactic["controller_y5a"] = new Controller(this, team["they"]->at(4), 1, 3000); //controle no referencial do ca
    tactic["controller_y6a"] = new Controller(this, team["they"]->at(5), 1, 3000); //controle no referencial do ca

	tactic["controller1"] = new Controller(this, team["us"]->at(0), 1, 3000); //controle no referencial do campo

    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 0"),"controller_b1");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 1"),"controller_b2");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 2"),"controller_b3");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 3"),"controller_b5");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 5"),"controller_b6");

	ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 0"),"controller_y1");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 1"),"controller_y2");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 2"),"controller_y3");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 3"),"controller_y4");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 4"),"controller_y5");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 5"),"controller_y6");

    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 0 (absoluto)"),"controller_y1a");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 1 (absoluto)"),"controller_y2a");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 2 (absoluto)"),"controller_y3a");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 3 (absoluto)"),"controller_y4a");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 4 (absoluto)"),"controller_y5a");
    ui.cmbSelectTacticTheirs->addItem(QStringLiteral("Controlar robô 5 (absoluto)"),"controller_y6a");

	ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 0 (absoluto)"),"controller_b1a");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 1 (absoluto)"),"controller_b2a");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 2 (absoluto)"),"controller_b3a");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 3 (absoluto)"),"controller_b4a");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 4 (absoluto)"),"controller_b5a");
    ui.cmbSelectTacticOurs->addItem(QStringLiteral("Controlar robô 5 (absoluto)"),"controller_b6a");

#endif
	tactic["zickler43"] = new Zickler43(this, team["us"]->at(4), 3000, true);
	tactic["gkpr"] = new Goalkeeper(this, team["us"]->at(0),3000);
    tactic["defU32"] = new Defender(this, team["us"]->at(3), team["they"]->at(2),team["us"]->goal(), 500, 3000);

	tactic["zickler43T"] = new Zickler43(this, team["they"]->at(4), 3000, true);
	tactic["gkprT"] = new Goalkeeper(this, team["they"]->at(0),3000);
    tactic["defT32"] = new Defender(this, team["they"]->at(3), team["us"]->at(2),team["they"]->goal(), 500, 3000);

	ui.stageView->setStage(stage["main"]);
	timer = new QTimer(this);

    // Fill dynamic dropdowns
    ui.cmbSelectTacticOurs->addItem("Zickler 43","zickler43");
    ui.cmbSelectTacticOurs->addItem("Goleiro","gkpr");
    ui.cmbSelectTacticOurs->addItem("Defesa 1","defU32");

    ui.cmbSelectTacticTheirs->addItem("Zickler 43","zickler43T");
    ui.cmbSelectTacticTheirs->addItem("Goleiro","gkprT");
    ui.cmbSelectTacticTheirs->addItem("Defesa 1","defT32");

    ui.cmbSelectPlayOurs->addItem("Halt","haltU");
    ui.cmbSelectPlayOurs->addItem("CBR2011","cbr");
    ui.cmbSelectPlayOurs->addItem(QStringLiteral("Retaliação"),"retaliateU");
    ui.cmbSelectPlayOurs->addItem("Minmax","minimax2");
    ui.cmbSelectPlayOurs->addItem("Obedecer juiz","refereeU");

    ui.cmbSelectMode->addItem("Play","PLAY");
    ui.cmbSelectMode->addItem(QStringLiteral("Tática"),"TACTIC");
    ui.cmbSelectMode->addItem("Skill","SKILL");
    ui.cmbSelectPlayTheirs->addItem("Halt","haltT");
    ui.cmbSelectPlayTheirs->addItem("CBR2011","cbr2");
    ui.cmbSelectPlayTheirs->addItem(QStringLiteral("Retaliação"),"retaliateT");
    ui.cmbSelectPlayTheirs->addItem("Obedecer juiz","refereeT");

	// TODO: automate this crap
    connect(ui.kickAbilityT0, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityT1, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityT2, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityT3, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityT4, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityT5, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityU0, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityU1, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityU2, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityU3, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityU4, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    connect(ui.kickAbilityU5, SIGNAL(returnPressed()), this, SLOT(setRobotKickAbility()));
    

    connect(ui.cmbRobot_0, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbRobot_1, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbRobot_2, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbRobot_3, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbRobot_4, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbRobot_5, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));

    connect(ui.cmbAdversary_0, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbAdversary_1, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbAdversary_2, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbAdversary_3, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbAdversary_4, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
    connect(ui.cmbAdversary_5, SIGNAL(currentIndexChanged(int)),
            this, SLOT(resetPatterns()));
	// ENDTODO
    //Connect signals to slots
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(ui.actionEditar_vari_veis_de_estado, SIGNAL(triggered()), alterStateVarsWindow, SLOT(show()));
    connect(ui.cmbPenalty, SIGNAL(currentIndexChanged(int)),this, SLOT(setPenaltyKicker()));
	connect(ui.cmbGoalkeeper, SIGNAL(currentIndexChanged(int)),this, SLOT(setGoalkeeper()));
	connect(ui.cmbSelectOutput, SIGNAL(currentIndexChanged(int)),this, SLOT(changeIntelligenceOutput()));
    connect(ui.cmbSelectPlayOurs, SIGNAL(currentIndexChanged(int)),this, SLOT(changePlayUs()));
    connect(ui.cmbSelectTacticOurs, SIGNAL(currentIndexChanged(int)),this, SLOT(changeTacticUs()));
    connect(ui.cmbSelectPlayTheirs, SIGNAL(currentIndexChanged(int)),this, SLOT(changePlayThem()));
    connect(ui.cmbSelectTacticTheirs, SIGNAL(currentIndexChanged(int)),this, SLOT(changeTacticThem()));
    connect(ui.cmbSelectMode, SIGNAL(currentIndexChanged(int)),this, SLOT(changeMode()));
    connect(ui.cmbOurTeam, SIGNAL(currentIndexChanged(int)), this, SLOT(setTeamColor()));
    connect(ui.btnChangeSides, SIGNAL(clicked()), this, SLOT(changeSides()));
    timer->start(10.);

    resetPatterns();

	team["us"]->at(5)->kicker().setWorking(0);
	//team["us"]->at(1)->kicker().setWorking(1);
}

void GraphicalIntelligence::setGoalkeeper()
{
	((ObeyReferee *) play["refereeU"])->setGoalkeeper(team["us"]->at(ui.cmbGoalkeeper->currentText().toInt()));
	((AutoRetaliate *) play["retaliateU"])->setGoalkeeper(team["us"]->at(ui.cmbGoalkeeper->currentText().toInt()));
	((ObeyReferee *) play["refereeT"])->setGoalkeeper(team["they"]->at(ui.cmbGoalkeeper->currentText().toInt()));
	((AutoRetaliate *) play["retaliateT"])->setGoalkeeper(team["they"]->at(ui.cmbGoalkeeper->currentText().toInt()));
}
void GraphicalIntelligence::setPenaltyKicker()
{
	int id = ui.cmbPenalty->currentText().toInt();
	((ObeyReferee *) play["refereeU"])->setPenaltyKicker(team["us"]->at(id));
	((ObeyReferee *) play["refereeT"])->setPenaltyKicker(team["they"]->at(id));
}

void GraphicalIntelligence::setRobotKickAbility()
{
	// TODO: Automate repeated bits of code to allow for more/less bots.
	bool* b = new bool(false);
	double d;
    //TODO: Put a slider here for maximum prettiness
	d = ui.kickAbilityU0->text().toDouble(b);
	if(*b)
	    team["us"]->at(0)->kicker().setWorking(d);
	d = ui.kickAbilityU1->text().toDouble(b);
	if(*b)
        team["us"]->at(1)->kicker().setWorking(d);
	d = ui.kickAbilityU2->text().toDouble(b);
	if(*b)
        team["us"]->at(2)->kicker().setWorking(d);
	d = ui.kickAbilityU3->text().toDouble(b);
	if(*b)
        team["us"]->at(3)->kicker().setWorking(d);
	d = ui.kickAbilityU4->text().toDouble(b);
	if(*b)
        team["us"]->at(4)->kicker().setWorking(d);
	d = ui.kickAbilityU5->text().toDouble(b);
	if(*b)
        team["us"]->at(5)->kicker().setWorking(d);
	
	d = ui.kickAbilityT0->text().toDouble(b);
	if(*b)
        team["they"]->at(0)->kicker().setWorking(d);
	d = ui.kickAbilityT1->text().toDouble(b);
	if(*b)
        team["they"]->at(1)->kicker().setWorking(d);
	d = ui.kickAbilityT2->text().toDouble(b);
	if(*b)
        team["they"]->at(2)->kicker().setWorking(d);
	d = ui.kickAbilityT3->text().toDouble(b);
	if(*b)
        team["they"]->at(3)->kicker().setWorking(d);
	d = ui.kickAbilityT4->text().toDouble(b);
	if(*b)
        team["they"]->at(4)->kicker().setWorking(d);
	d = ui.kickAbilityT5->text().toDouble(b);
	if(*b)
        team["they"]->at(5)->kicker().setWorking(d);
}


void GraphicalIntelligence::changePatternId()
{
    this->resetPatterns();
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
    delete timer;
//    delete cli;
}

void GraphicalIntelligence::resetPatterns()
{
    if(useSimulation) {
        for(int i = 0; i < team["us"]->size(); i++)
            team["us"]->at(i)->setPatternId(i);
        for(int i = 0; i < team["they"]->size(); i++)
            team["they"]->at(i)->setPatternId(i);
    } else {
// TODO: Automate repeated bits of code to allow for more/less bots.
        team["us"]->at(0)->setPatternId(ui.cmbRobot_0->currentText().toInt());
        team["us"]->at(1)->setPatternId(ui.cmbRobot_1->currentText().toInt());
        team["us"]->at(2)->setPatternId(ui.cmbRobot_2->currentText().toInt());
        team["us"]->at(3)->setPatternId(ui.cmbRobot_3->currentText().toInt());
        team["us"]->at(4)->setPatternId(ui.cmbRobot_4->currentText().toInt());
        team["us"]->at(5)->setPatternId(ui.cmbRobot_5->currentText().toInt());

        team["they"]->at(0)->setPatternId(ui.cmbAdversary_0->currentText().toInt());
        team["they"]->at(1)->setPatternId(ui.cmbAdversary_1->currentText().toInt());
        team["they"]->at(2)->setPatternId(ui.cmbAdversary_2->currentText().toInt());
        team["they"]->at(3)->setPatternId(ui.cmbAdversary_3->currentText().toInt());
        team["they"]->at(4)->setPatternId(ui.cmbAdversary_4->currentText().toInt());
        team["they"]->at(5)->setPatternId(ui.cmbAdversary_5->currentText().toInt());

    }
}

//TODO
QObject* GraphicalIntelligence::getObjectFromNumberedId(int id, QString s)
{
    //Returns the object with name: s concatenated with the numeric id;
    //return findChild(s + QString(id));
    return NULL;
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
			updater["refereeSim"]->step();
			updater["refereeSim"]->apply();
		} else {
			updater["vision"]->step();
			updater["vision"]->apply();
			updater["referee"]->step();
			updater["referee"]->apply();
		}

		///BEGIN STEPS
		switch(mode) {

		case PLAY:
            //play["cbr"]->step();
            //play["cbr2"]->step();
			//if(!((QThread *)play["minimax2"])->isRunning())
			//	((QThread *)play["minimax2"])->start();
			//play["minimax2"]->step();
            //play["refereeU"]->step();
            //play["refereeT"]->step();
			//play["stoprefT"]->step();
            //play["retaliateT"]->step();
            //play["retaliateU"]->step();
			//tactic["zickler43"]->step();
			//play["retaliateT"]->step();
            
			if(current_play_us != NULL)
            {
                if(current_play_us != play["minmax2"])
                {
                    current_play_us->step();
                }
                else
                {
                    if(!((QThread *)play["minimax2"])->isRunning())
                        ((QThread *)play["minimax2"])->start();
                }
            }
            if(current_play_them != NULL)
            {
                current_play_them->step();
            }
			break;

		case TACTIC:
            /*
			tactic["attackerM"]->step();
			tactic["zickler43"]->step();
			//tactic["gkpr"]->step();
			//tactic["def"]->step();
            */
            if(current_tactic_us != NULL)
            {
               current_tactic_us->step();
            }
            if(current_tactic_them != NULL)
            {
               current_tactic_them->step();
            }
			break;

		case SKILL:
			//skill["fac"]->step();
			skill["goto"]->step();
			skill["drivetoBall"]->step();
			skill["samd"]->step();
			//cout << skill["gotoa"]->busy() << endl;
			//skill["gotoa"]->step();
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
            //XXX: CommanderSim resets the robots' command, while CommanderGrSim doesn't.
            // As such, when using both, always dispatch GrSim commands first!
			commander["blueGrSim"]->step();
			commander["blueSim"]->step();

            ((CommanderGrSim*)commander["blueGrSim"])->send();
            ((CommanderSim*)commander["blueSim"])->send();
            
#endif
#ifdef CONTROL_YELLOW
            //XXX: CommanderSim resets the robots' command, while CommanderGrSim doesn't.
            // As such, when using both, always dispatch GrSim commands first!
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
	//UpdaterReferee* updR = qobject_cast<UpdaterReferee*>(updater["vision"]);//SA: Dead store, possible BUG



	/// Timeouts
	//ui.txtNrTimeouts->setText(QString::number(stage["main"]->));
	//ui.txtTempoTimeouts->setText(QString::number(stage["main"]->));

	ui.txtStageLenght->setText(QString::number(stage["main"]->fieldLength()));
	ui.txtStageWidth->setText(QString::number(stage["main"]->fieldWidth()));

	ui.stageView->redraw();
}


/*void GraphicalIntelligence::on_btnIntStart_clicked()
{
	timer->start();
}

void GraphicalIntelligence::on_btnIntStop_clicked()
{
	timer->stop();
}*/

void GraphicalIntelligence::changeIntelligenceOutput()
{
    if (this->ui.cmbSelectOutput->currentIndex()==0)
    {
        //cout << "Using simulation." << endl;
        mutex.lock();
        useSimulation = true;
        resetPatterns();
        mutex.unlock();
    }
    else
    {
        //cout << "Using real transmission/SSL." << endl;
        mutex.lock();
        useSimulation = false;
        //resetPatterns();
        mutex.unlock();
    }
}

void GraphicalIntelligence::changePlayUs()
{
    int current_index = ui.cmbSelectPlayOurs->currentIndex();
    mutex.lock();
    //cout << "Initiating play " << ui.cmbSelectPlayOurs->itemData(current_index).toString().toStdString() << " for our team:" << endl;
    this->current_play_us = play[ui.cmbSelectPlayOurs->itemData(current_index).toString().toStdString()];
    mutex.unlock();

}

void GraphicalIntelligence::changePlayThem()
{
    int current_index = ui.cmbSelectPlayTheirs->currentIndex();
    mutex.lock();
    //cout << "Initiating play " << ui.cmbSelectPlayTheirs->itemData(current_index).toString().toStdString() << " for their team:" << endl;
    this->current_play_them = play[ui.cmbSelectPlayTheirs->itemData(current_index).toString().toStdString()];
    mutex.unlock();
}

void GraphicalIntelligence::changeTacticUs()
{
    int current_index = ui.cmbSelectTacticOurs->currentIndex();
    mutex.lock();
    //cout << "Initiating Tactics " << ui.cmbSelectTacticsOurs->itemData(current_index).toString().toStdString() << " for our team:" << endl;
    this->current_tactic_us = tactic[ui.cmbSelectTacticOurs->itemData(current_index).toString().toStdString()];
    mutex.unlock();

}

void GraphicalIntelligence::changeTacticThem()
{
    int current_index = ui.cmbSelectTacticTheirs->currentIndex();
    mutex.lock();
    //cout << "Initiating Tactics " << ui.cmbSelectTacticsTheirs->itemData(current_index).toString().toStdString() << " for their team:" << endl;
    this->current_tactic_them = tactic[ui.cmbSelectTacticTheirs->itemData(current_index).toString().toStdString()];
    mutex.unlock();
}

void GraphicalIntelligence::changeMode()
{
    int current_index = ui.cmbSelectMode->currentIndex();
    mutex.lock();
    if (ui.cmbSelectMode->itemData(current_index).toString().toStdString() == "PLAY")
    {
        this->mode = PLAY;
    }
    if (ui.cmbSelectMode->itemData(current_index).toString().toStdString() == "TACTIC")
    {
        this->mode = TACTIC;
    }
    if (ui.cmbSelectMode->itemData(current_index).toString().toStdString() == "SKILL")
    {
        this->mode = SKILL;
    }
    mutex.unlock();
}

void GraphicalIntelligence::changeSides()
{
    stage["main"]->setIsLeftSideBlueGoal(!stage["main"]->isLeftSideBlueGoal());
}

void GraphicalIntelligence::setTeamColor()
{
    bool we_should_be_blue = (ui.cmbOurTeam->currentText() == "Azul");

    //cout << "Devemos ser azul?" << we_should_be_blue << endl;

    if((we_should_be_blue && team["us"]->last()->color()==BLUE)|| (!we_should_be_blue && team["us"]->last()->color()==YELLOW))
    {
        return;
    }

    LibIntelligence::TeamColor our_colour, their_colour;

    if(we_should_be_blue)
    {

        //Team* t_us = team["us"];
        //t_us->setColor(BLUE);
        //Team* t_them = team["they"];
        //t_them->setColor(YELLOW);
        //our_colour = BLUE; their_colour=YELLOW;
		
        //stage["main"]->setBlueTeam(t_us);
        //stage["main"]->setYellowTeam(t_them);
    }
    else
    {
        //Team* t_us = team["us"];
        //t_us->setColor(YELLOW);
        //Team* t_them = team["they"];
        //t_them->setColor(BLUE);
        //our_colour = YELLOW; their_colour=BLUE;
        //stage["main"]->setYellowTeam(t_us);
        //stage["main"]->setBlueTeam(t_them);
    }
    for(int i=0; i<team["us"]->count(); i++)
    {
        //team["us"]->at(i)->setColor(our_colour);
    }
    for(int i=0; i<team["they"]->count(); i++)
    {
        //team["they"]->at(i)->setColor(their_colour);
    }
    resetPatterns();
}

void GraphicalIntelligence::load_configs()
{
    QSettings settings("settings.ini", QSettings::IniFormat, this);
}
