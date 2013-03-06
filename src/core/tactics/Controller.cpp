#ifdef HAVE_WINDOWS
#include "Controller.h"
#include "CXBOXController.h"
#include "Robot.h"
#include <iostream>
#include <cmath>
#include "Tactics.h"
#include "Skills.h"

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Controller::Controller(QObject* p, Robot* r, int i, qreal s)
	: Tactic(p,r),
	id(i),
	speed(s)
{
	steer = new SteerToBall(this, r, s, s);
	bl = new Blocker(this, robot(), 0, s);
	zk = new Zickler43(this, robot(), s, true);
	mv = new Move(this, robot(), 0.0, 0.0, 0.0);
	gk = new Goalkeeper(this, robot(), s);
	controller = new CXBOXController(id);
	pushState(steer);
	pushState(mv);
}

void Controller::setSpeed(qreal speed)
{
	this->speed = speed;
	bl->setSpeed(speed);
	zk->setSpeed(speed);
	gk->setSpeed(speed);
}

void Controller::setRobot(Robot* r)
{
	Tactic::setRobot(r);
	bl->setRobot(r);
	zk->setRobot(r);
	gk->setRobot(r);
}

void Controller::step() {
	qreal sx = 0.0, sy = 0.0, dx = 0.0, dy = 0.0, sBoost = 1.0, aBoostAng = 1.0;
	qint32 dribSign = 1;

	if(controller->IsConnected()){
		//rotation
		qreal dx = controller->ThumbRX();
		qreal dy = controller->ThumbRY();
		if(dx * dx + dy * dy < .05) {
			dx = 0;
			dy = 0;
		}
		if(controller->ButtonPressed(XINPUT_GAMEPAD_RIGHT_THUMB)) {
			aBoostAng = 2.0;
		} else {
			aBoostAng = 1.0;
		}
		steer->setRate(1.5 * aBoostAng);//PARAMETRO CARTEADO

		//movement
		sx = controller->ThumbLX();
		sy = controller->ThumbLY();
		// dead zone
		if(sx * sx + sy * sy < .05) {
			sx = 0;
			sy = 0;
		}
		if(controller->ButtonPressed(XINPUT_GAMEPAD_LEFT_THUMB)) {
			sBoost = 2.0;
		} else {
			sBoost = 1.0;
		}
		steer->setAll(sx * sBoost * speed, sy * sBoost * speed, dx, dy);
		steer->step();

		//dribbler
		robot()->dribble(dribSign * controller->TriggerL());

		//kicker
		if(controller->ButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			robot()->kick(controller->TriggerR());
		} else {
			robot()->kick(0.0);
		}

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

		else{
			//direcionais
			Move mv(this, robot(), 0.0, 0.0, 0.0);
			if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_DOWN)) {
				mv.setAll(0.0, -speed, 0.0);
				mv.step();
			}
			if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_UP)) {
				mv.setAll(0.0, speed, 0.0);
				mv.step();
			}
			if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_LEFT)) {
				mv.setAll(-speed, 0.0, 0.0);
				mv.step();
			}
			if(controller->ButtonPressed(XINPUT_GAMEPAD_DPAD_RIGHT)) {
				mv.setAll(speed, 0.0, 0.0);
				mv.step();
			}
		}
	}
}

#endif

