#ifdef HAVE_WINDOWS
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "LibIntelligence.h"
#include "Tactic.h"
//#include "Steer.h"
#include "SteerToBall.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class Controller : public Tactic
		{
		public:
			Controller(QObject* parent, Robot* slave, int controllerId, qreal speed);

			void step();

		protected:
			int id;
			qreal speed;
			//Skills::Steer* steer;
			Skills::SteerToBall* steer;
		};
	}	
}

#endif // CONTROLLER_H
#endif

