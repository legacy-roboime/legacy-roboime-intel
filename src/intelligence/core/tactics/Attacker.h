#pragma once
#ifndef ATTACKER_H
#define ATTACKER_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Goto.h"
#include "GetBall.h"
#include "KickTo.h"
#include "Skills.h"
#include "Blocker.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class Attacker : public Blocker //OBSOLETE CODE
		{
			Q_OBJECT

		public:
			Attacker(QObject* parent, Robot* slave, qreal speed);

			void step();

		protected:
			Skills::Goto* goto_;
			Skills::GetBall* getBall_;
			Skills::KickTo* kickTo_;
			Skills::Loops::Orbit* orbit_;
			Skills::Move* move_;
			qreal speed;
		};
	}	
}

#endif // ATTACKER_H
