#ifdef HAVE_WINDOWS
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "LibIntelligence.h"
#include "SteerToPoint.h"
#include "Blocker.h"
#include "GoalSwitcheroo.h"
#include "Goalkeeper.h"
#include "Zickler43.h"
#include "Goto.h"
#include "Move.h"
#include "CXBOXController.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class Controller : public Tactic
		{
			Q_OBJECT

		public:
			Controller(QObject* parent, Robot* slave, int controllerId, qreal speed);
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
			Skills::Move* mv;
			CXBOXController* controller;
		};
	}	
}

#endif // CONTROLLER_H
#endif

