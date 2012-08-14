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
	bl = new Blocker(this, robot(), 0, speed);
	gs = new GoalSwitcheroo(this, robot(), speed);
	zk = new Zickler43(this, robot(), speed, true);
	mv = new Move(this, robot(), 0.0, 0.0, 0.0);
	gt = new Goto(this, robot(), 0.0, 0.0);
	gk = new Goalkeeper(this, robot(), speed);
	controller = new CXBOXController(id);
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
		//if(controller->ButtonPressed(XINPUT_

		//dribbler
		robot()->dribble(dribSign * controller->TriggerL());

		//kicker
		if(controller->ButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			robot()->kick(controller->TriggerR());
		} else {
			robot()->kick(0.0);
		}
		steer->step();

		//blocker
		
		if(controller->ButtonPressed(XINPUT_GAMEPAD_A)) {
			bl->step();
		}

		//goto
		
		if(controller->ButtonPressed(XINPUT_GAMEPAD_B)) {
			gs->step();
		}

		//goalkeeper
		if(controller->ButtonPressed(XINPUT_GAMEPAD_X)) {
			gk->step();
		}

		//zickler
		if(controller->ButtonPressed(XINPUT_GAMEPAD_Y)) {
			zk->step();
			robot()->dribble(0);
		}

		//goto
		gt->setAll(0.0, 0.0, speed);
		gt->setOrientation(dx, dy);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			gt->step();
		}

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

#endif

