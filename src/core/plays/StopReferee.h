#pragma once
#ifndef STOPREFEREE_H
#define STOPREFEREE_H

#include "LibIntelligence.h"
#include <QObject>
#include "Play.h"
#include "Tactics.h"

namespace LibIntelligence
{
	namespace Plays{

		class StopReferee : public Play
		{
			Q_OBJECT

		public:
			StopReferee(QObject*, Team*,Stage*, Robot* gk);
			~StopReferee();
			virtual void step();
            void setGoalkeeper(Robot* gk);

		private:
			Object* cover1;
			Object* cover2;

			bool init;
		};
	}
}

#endif // STOPREFEREE_H
