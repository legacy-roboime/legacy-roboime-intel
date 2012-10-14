#ifdef HAVE_WINDOWS
#include "Controller2.h"
#include "CXBOXController.h"
#include "Move.h"
#include "Robot.h"
#include "Tactics.h"
#include "Skills.h"
#include <iostream>
#include <cmath>

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Controller2::Controller2(QObject* p, Robot* r, int i, qreal s)
	: Tactic(p,r),
	id(i),
	speed(s),
	move(new Move(this, r))
{
	steer = new SteerToBall(this, r, s, s);
	bl = new Blocker(this, robot(), 0, s);
	zk = new Zickler43(this, robot(), s, true);
	gk = new Goalkeeper(this, robot(), s);
	pushState(steer);
	this->pushState(move);//this is important
}

void Controller2::setSpeed(qreal speed)
{
	this->speed = speed;
	bl->setSpeed(speed);
	zk->setSpeed(speed);
	gk->setSpeed(speed);
}

void Controller2::setRobot(Robot* r)
{
	Tactic::setRobot(r);
	bl->setRobot(r);
	gk->setRobot(r);
	zk->setRobot(r);
}

void Controller2::step() {
	CXBOXController* controller = new CXBOXController(id);
	qreal sx = 0.0, sy = 0.0, dx = 0.0, dy = 0.0, sBoost = 1.0, aBoostAng = 1.0;
	qint32 dribSign = 1;

	if(controller->IsConnected()){
		//rotation
		qreal t_dx = controller->ThumbRX();
		qreal t_dy = controller->ThumbRY();

		if(t_dx * t_dx + t_dy * t_dy < .05) {
			t_dx = 0;
			t_dy = 0;
		}
		if(controller->ButtonPressed(XINPUT_GAMEPAD_RIGHT_THUMB)) {
			aBoostAng = 5.0;
		} else {
			aBoostAng = 1.0;
		}
		move->setSpeedAngular(-t_dx * 5);
		//steer->setRate(0.06 * aBoostAng);

		//movement
		sx = controller->ThumbLX();
		sy = controller->ThumbLY();
		// dead zone
		if(sx * sx + sy * sy < .05) {
			sx = 0;
			sy = 0;
		}

		//direcionais
		if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_DOWN)) {
			sx = 0;
			sy = -1;
		}
		if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_UP)) {
			sx = 0;
			sy = 1;
		}
		if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_LEFT)) {
			sx = -1;
			sy = 0;
		}
		if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			sx = 1;
			sy = 0;
		}

		if(controller->ButtonPressed(XINPUT_GAMEPAD_LEFT_THUMB)) {
			sBoost = 10.0;
		} else {
			sBoost = 1.0;
		}
		//steer->setAll(sx * sBoost * speed, sy * sBoost * speed, dx, dy);
		qreal theta = robot()->orientation();
		qreal ssx = sx * cos(theta) - sy * sin(theta);
		qreal ssy = sx * sin(theta) + sy * cos(theta);
		//steer->setAll(ssx * sBoost * speed, ssy * sBoost * speed, dx, dy);

		move->setSpeeds(ssy * sBoost * speed, - ssx * sBoost * speed);

		//dribbler
		if(controller->ButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			dribSign = -1;
		} else {
			dribSign = 1;
		}
		robot()->dribble(dribSign * controller->TriggerL());

		//kicker
		if(controller->ButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			robot()->kick(controller->TriggerR());
		} else {
			robot()->kick(0.0);
		}
		//steer->step();

		//blocker
		if(controller->ButtonPressed(XINPUT_GAMEPAD_A)) {
			bl->step();
		}
		
		else if(controller->ButtonPressed(XINPUT_GAMEPAD_B)) {
			Team* team = robot()->team();
			int id = robot()->id();
			id++;
			id%=team->size();
			this->setRobot(team->at(id));
		}

		//goalkeeper
		else if(controller->ButtonPressed(XINPUT_GAMEPAD_X)) {
			gk->step();
		}

		//zickler
		else if(controller->ButtonPressed(XINPUT_GAMEPAD_Y)) {
			zk->step();
		}

		else 
			move->step();
	}
}

#endif

