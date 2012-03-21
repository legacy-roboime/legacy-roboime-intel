#pragma once
#ifndef DEFENDER_H
#define DEFENDER_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Goto.h"
#include "GetBall.h"
#include "KickTo.h"


namespace LibIntelligence
{
	namespace Tactics
	{
		class Defender : public Tactic
		{
			Q_OBJECT

		public:
			Defender(QObject* parent, Robot* slave, int position, qreal speed);

			void step();

		protected:
			int position_;
			Skills::Goto* goto_;
			Skills::GetBall* getBall_;
			Skills::KickTo* kickTo_;
		};
	}	
}

#endif // DEFENDER_H