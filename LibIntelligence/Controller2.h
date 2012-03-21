#pragma once
#ifndef CONTROLLER2_H
#define CONTROLLER2_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Move.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class Controller2 : public Tactic
		{
		public:
			Controller2(QObject* parent, Robot* slave, int controllerId, qreal speed);

			void step();

		protected:
			int id;
			qreal speed;
			Skills::Move* move;
		};
	}	
}

#endif // CONTROLLER_H
