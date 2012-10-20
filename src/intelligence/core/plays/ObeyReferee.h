#ifndef OBEYREFEREE_H
#define OBEYREFEREE_H

#include "Play.h"

namespace LibIntelligence
{
	namespace Plays
	{
		class ObeyReferee : public Play
		{
			Q_OBJECT

		public:
			ObeyReferee(QObject *parent, Play *, Robot* goalkeeper, Robot* penaltyKicker);
			~ObeyReferee();

			void step();
			void setPlay(Play *);
			void setTeam(Team *);
			void setStage(Stage *);
            void setGoalkeeper(Robot* gk);
			void setPenaltyKicker(Robot* pk);
		private:
			Play *play;
			Play *stopReferee;
			Play *halt;
			Play *penaltyUs;
			Play *penaltyThem;
			Play *indirectKick;
			char cmd;
			char lastCmd;
			QPointF lastBall;
		};
	}
}

#endif // OBEYREFEREE_H
