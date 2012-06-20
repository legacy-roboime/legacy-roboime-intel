#include "Controller.h"
#include "CXBOXController.h"
#include "Tactics.h"
#include "Skills.h"
#include "Robot.h"
#include <iostream>
#include <cmath>

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Controller::Controller(QObject* p, Robot* r, int i, qreal s)
	: Tactic(p,r),
	id(i),
	speed(s),
	steer(this, r)
{
	this->pushState(&steer);
	//skills.append(steer);//this is important
}

void Controller::step() {
	//FIXME: tirar statics daqui
	//FIXME: reimplementar setRobot para chamar os setRobots das skills respectivas
	static CXBOXController* controller = new CXBOXController(id);
	static qreal sx = 0.0, sy = 0.0, dx = 0.0, dy = 0.0, sBoost = 1.0, aBoostAng = 1.0;
	static qint32 dribSign = 1;

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
		steer.setRate(1.5 * aBoostAng);//PARAMETRO CARTEADO

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
		steer.setAll(sx * sBoost * speed, sy * sBoost * speed, dx, dy);
		//if(controller->ButtonPressed(XINPUT_

		//dribbler
		robot()->dribble(dribSign * controller->TriggerL());

		//kicker
		if(controller->ButtonPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			robot()->kick(controller->TriggerR());
		} else {
			robot()->kick(0.0);
		}
		steer.step();

		//blocker
		static Blocker bl = Blocker(this, robot(), 0, speed);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_A)) {
			bl.step();
		}

		//goto
		static GoalSwitcheroo gs(this, robot(), speed);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_B)) {
			gs.step();
		}

		//goalkeeper
		static Goalkeeper gk(this, robot(), speed);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_X)) {
			gk.step();
		}

		//zickler
		static Zickler43 zk = Zickler43(this, robot(), speed, true);
		//static IndirectKick atk(this, robot(),speed,500);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_Y)) {
			zk.step();
			//robot()->dribble(0);
		}

		//goto 0.0 0.0
		//static GotoOld gt(this, robot(), 0.0, 0.0);
		static Goto gt(this, robot(), 0.0, 0.0);
		gt.setAll(0.0, 0.0, speed);
		gt.setOrientation(dx, dy);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			gt.step();
		}

		//direcionais
		static Move mv(this, robot(), 0.0, 0.0, 0.0);
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