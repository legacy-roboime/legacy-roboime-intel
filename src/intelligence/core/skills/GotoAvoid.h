#ifndef GOTOAVOID_H
#define GOTOAVOID_H

#include "Goto.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class GotoAvoid : protected Goto
		{
		public:
			GotoAvoid(QObject *parent, Robot *slave, qreal targetX, qreal targetY, qreal avoidX, qreal avoidY, qreal speed);

			void setTarget(qreal x, qreal y);
			void setAvoid(qreal x, qreal y);
			void setSpeed(qreal);

			void step();

		private:
			qreal avoidX, avoidY;
		};
	}
}

#endif