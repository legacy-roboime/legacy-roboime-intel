#pragma once
#ifndef GOALKEEPER_H
#define GOALKEEPER_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Goto.h"
#include "GetBall.h"
#include "KickTo.h"


namespace LibIntelligence
{
	namespace Tactics
	{
		class Goalkeeper : public Tactic
		{
			Q_OBJECT

		public:
			Goalkeeper(QObject* parent, Robot* slave, qreal speed);
			bool busy();
			void step();
			void setSpeed(qreal speed);

		protected:
			Skills::Goto* goto_;
		};
	}	
}

#endif // GOALKEEPER_H
