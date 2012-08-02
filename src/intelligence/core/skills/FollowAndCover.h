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
			FollowAndCover(QObject *parent, Robot *slave, Point *follow, Point *cover, qreal followDistance, qreal speed);

			void setFollow(Point *);
			void setCover(Point *);
			void setDistance(qreal);
			void setSpeed(qreal);

			void step();

		private:
			Point *follow;
			Point *cover;
			qreal distance;
		};
	}
}

#endif