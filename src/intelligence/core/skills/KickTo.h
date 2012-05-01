#pragma once
#ifndef KICKTO_H
#define KICKTO_H

#include "LibIntelligence.h"
#include "GetBall.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class KickTo : public GetBall
		{
			Q_OBJECT

		public:
			KickTo(QObject* parent, Robot* slave, qreal angle=0.0);
			void step();
			void setAngle(qreal _angle);
			qreal getAngle();

		protected:
			qreal angle;
		};
	}
}

#endif // KICKTO_H
