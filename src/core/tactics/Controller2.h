#ifdef HAVE_WINDOWS
#ifndef CONTROLLER2_H
#define CONTROLLER2_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "SteerToPoint.h"
#include "Blocker.h"
#include "GoalSwitcheroo.h"
#include "Goalkeeper.h"
#include "Zickler43.h"
#include "Goto.h"
#include "Move.h"
#include "Attacker.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class Controller2 : public Tactic
		{
		public:
			Controller2(QObject* parent, Robot* slave, int controllerId, qreal speed);
			void setSpeed(qreal speed);
			void step();
			void setRobot(Robot* r);

		protected:
			int id;
			qreal speed;
			Skills::SteerToBall* steer;
			Tactics::Blocker* bl;
			Tactics::Goalkeeper* gk;
			Tactics::Zickler43 *zk;
			Skills::Move* move;
			Skills::KickTo *kt;
			Tactics::Attacker* at;
		};
	}	
}

#endif // CONTROLLER_H
#endif

