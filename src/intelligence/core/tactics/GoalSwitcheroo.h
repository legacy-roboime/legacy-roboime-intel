#ifndef GOALSWITCHEROO_H
#define GOALSWITCHEROO_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Skills.h"

namespace LibIntelligence
{
	class Robot;
	namespace Tactics
	{
		class GoalSwitcheroo : public Tactic
		{
			Q_OBJECT
		public:
			GoalSwitcheroo(QObject *parent, Robot* slave, qreal speed);
			~GoalSwitcheroo();
			void step();
		protected:
			Skills::Goto* goto_;

		private:
			bool atacando;
		};
	}
}
#endif // GOALSWITCHEROO_H
