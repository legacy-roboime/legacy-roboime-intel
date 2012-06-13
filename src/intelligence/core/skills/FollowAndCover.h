#ifndef FOLLOWANDCOVER_H
#define FOLLOWANDCOVER_H

#include "Goto.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class FollowAndCover : public Goto
		{
		public:
			FollowAndCover(QObject *parent, Robot *slave, QPointF *follow, QPointF *cover, qreal followDistance, qreal speed);

			void setFollow(QPointF *);
			void setCover(QPointF *);
			void setDistance(qreal);
			void setSpeed(qreal);

			void step();

		private:
			QPointF *follow;
			QPointF *cover;
			qreal distance;
		};
	}
}

#endif