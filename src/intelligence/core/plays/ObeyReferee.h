#ifndef OBEYREFEREE_H
#define OBEYREFEREE_H

#include "Play.h"
#include "KickTo.h"
#include "Goal.h"

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
			Skills::KickTo* kickTo;
			char cmd;
			char lastCmd;
			QPointF lastBall;
			Point penaltyTarget;
			Robot* pKicker_;
		};
	}
}

#endif // OBEYREFEREE_H
