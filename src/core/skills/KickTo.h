#pragma once
#ifndef KICKTO_H
#define KICKTO_H

#include "LibIntelligence.h"
#include "GetBall.h"
#include "PID.h"
#include "Steer.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class KickTo : public Steer
		{
			Q_OBJECT

		public:
			KickTo(QObject* parent, Robot* slave, qreal angle=0.0);
			void step();
			void setAngle(qreal _angle);
			qreal getAngle();
			void setPoint(Point p) { point_=p ;	aproachSpeed=0; }

		protected:
			Point point_;
			qreal angle;
			CONTROLLER_S controllerDist;
			qreal aproachSpeed;
		};
	}
}

#endif // KICKTO_H
