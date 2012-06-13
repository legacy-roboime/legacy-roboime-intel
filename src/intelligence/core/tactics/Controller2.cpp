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
	this->pushState(move);
	//skills.append(move);//this is important
}

void Controller2::step() {
	static CXBOXController* controller = new CXBOXController(id);
	static qreal sx = 0.0, sy = 0.0, dx = 0.0, dy = 0.0, sBoost = 1.0, aBoostAng = 1.0;
	static qint32 dribSign = 1;

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

		//getBall
		static GetBall gb(this, robot(), speed);
		gb.setSpeed(speed);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_A)) {
			gb.step();
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

		//goalkeeper
		static Attacker atk(this, robot(),1000);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_Y)) {
			atk.step();
		}

		//goto 0.0 0.0
		static Goto gt(this, robot(), 0.0, 0.0, 0.0, speed);
		gt.setOrientation(dx, dy);
		if(controller->ButtonPressed(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			gt.step();
		}

		move->step();
	}
}

#endif

