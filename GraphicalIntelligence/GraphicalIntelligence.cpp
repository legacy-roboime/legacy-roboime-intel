#include "GraphicalIntelligence.h"
#include <QTimer>
#include "Robot.h"
#include "Robots.h"
#include "CommanderTx.h"
#include "CommanderTxOld.h"
#include "CommanderSim.h"
#include "UpdaterVision.h"
#include "Skills.h"
#include "Ball.h"
#include "Stage.h"

using namespace LibIntelligence;

GraphicalIntelligence::GraphicalIntelligence(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags),
	sta(new Stage()),
	comB(new CommanderSim(this)),
	comB2(new CommanderTxOld(this)),
	comY(new CommanderSim(this)),
	upd(new UpdaterVision(this)),
	ball(new Ball(this, 21.5)),
	blueTeam(sta, BLUE),
	yellowTeam(sta, YELLOW)
{
	ui.setupUi(this);

	sta->setBall(ball);
	upd->add(ball);
	upd->add(sta);

	for(int i=0; i<5; i++) {
		blueTeam << new Robot(Robots::RobotIME2011(&blueTeam, i, i, BLUE));
		comB->add(blueTeam.last());
		comB2->add(blueTeam.last());
		upd->add(blueTeam.last());

		yellowTeam << new Robot(Robots::RobotIME2011(&yellowTeam, i + 6, i + 6, YELLOW));
		comY->add(yellowTeam.last());
		upd->add(yellowTeam.last());
	}

	//updReferee = new UpdaterReferee();

	//BLUE TEAM
	//test[0] = new Skills::Loops::Circle(this, blueTeam[0], -1200, 1500, 600, 1000);
	//test[0] = new Skills::Wait(this, blueTeam[0]);
	test[0] = new Skills::Goto(this, blueTeam[0], 1000, 500, 0.0, 1000);

	//test[1] = new Skills::Loops::Circle(this, blueTeam[1], 1200, -1500, -100, 1000);
	//test[1] = new Skills::Goto(this, blueTeam[1], 2000, 1000, 0, 1000);
	//test[1] = new Skills::Wait(this, blueTeam[1]);
	//test[1] = new Skills::Steer(this, blueTeam[1], 10.0, 10.0, 1.0);
	test[1] = new Skills::Goto(this, blueTeam[1], 1000, -500, 0.0, 1000);

	//test[2] = new Skills::Loops::Orbit(this, blueTeam[2], -1200, -1500, 800, 1000, 0.03);
	//test[2] = new Skills::Wait(this, blueTeam[2]);
	//test[2] = new Skills::Goto(this, blueTeam[2], 1000, 1000, 0.0, 1000);
	test[2] = new Skills::Steer(this, blueTeam[2], 0.0, 0.0, -3.1416);

	//test[3] = new Skills::Loops::Orbit(this, blueTeam[3], 1500, 1200, -300, 1000, 0.07);
	//test[3] = new Skills::Wait(this, blueTeam[3]);
	test[3] = new Skills::GetBall(this, blueTeam[3], 800);

	//test[4] = new Skills::Loops::Circle(this, blueTeam[4], 0, 0, 600, 3000);
	//test[4] = new Skills::Goto(this, blueTeam[4], 2000, -1000, 0, 1000);
	//test[4] = new Skills::Wait(this, blueTeam[4]);
	test[4] = new Skills::Goto(this, blueTeam[4], 2200, -1200, 0.0, 2000);
	//test[4] = new Controller(1, blueTeam[4], sta, 20);

	//YELLOW TEAM
	//test[5] = new Skills::Loops::Circle(this, yellowTeam[0], -1200, 1500, 600, 1000);
	test[5] = new Skills::Wait(this, yellowTeam[0]);

	//test[5] = new Skills::Loops::Circle(this, yellowTeam[1], 1200, -1500, -100, 1000);
	//test[6] = new Skills::Goto(this, yellowTeam[1], 2000, 1000, 0, 1000);
	test[6] = new Skills::Wait(this, yellowTeam[1]);

	test[7] = new Skills::Loops::Orbit(this, yellowTeam[2], -1200, -1500, 800, 1000, 0.03);
	//test[7] = new Skills::Wait(this, yellowTeam[2]);

	//test[8] = new Skills::Loops::Orbit(this, yellowTeam[3], 1500, 1200, -300, 1000, 0.07);
	test[8] = new Skills::Wait(this, yellowTeam[3]);

	//test[9] = new Skills::Loops::Circle(this, yellowTeam[4], 0, 0, 600, 3000);
	//test[9] = new Skills::Goto(this, yellowTeam[4], 2000, -1000, 0, 1000);
	test[9] = new Skills::Wait(this, yellowTeam[4]);
	//test[9] = new Controller(1, yellowTeam[4], sta, 20);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(30);
}

GraphicalIntelligence::~GraphicalIntelligence() {}

void GraphicalIntelligence::update() {
	upd->step();
	upd->apply();
	//updReferee->step();
	for(int i=0; i<10; i++) {
		test[i]->step();
	}
	comB->step();
	//comB2->step();
	comY->step();
}
