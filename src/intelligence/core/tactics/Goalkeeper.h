#pragma once
#ifndef GOALKEEPER_H
#define GOALKEEPER_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Goto.h"
//#include "GetBall.h"
//#include "KickTo.h"
#include "SampledKick.h"


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
            qreal holeSize();

		protected:
            Line homeline;
            Skills::Goto *goto_;
            Skills::SampledKick *kick;
            Point pointToKeep();
            bool isKickScored(Point kickPoint);
            qreal maxHoleSize;
		};
	}	
}

#endif // GOALKEEPER_H
