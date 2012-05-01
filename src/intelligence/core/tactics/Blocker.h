#pragma once
#ifndef BLOCKER_H
#define BLOCKER_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Goto.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class Blocker : public Tactic
		{
			Q_OBJECT

		public:
			Blocker(QObject* parent, Robot* slave, qreal angle, qreal speed=1000);

			void step();

		protected:
			Skills::Goto* goto_;
			qreal angle_;
			qreal speed;
		};
	}	
}

#endif // BLOCKER_H

