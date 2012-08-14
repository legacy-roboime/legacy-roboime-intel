#pragma once
#ifndef DRIVETO_H
#define DRIVETO_H

#include "LibIntelligence.h"
#include "Goto.h"
#include "Wait.h"

#define M_PI	3.1415926535897932

namespace LibIntelligence
{
	namespace Skills
	{
		class DriveTo : public Goto
		{
			Q_OBJECT

		public:
			DriveTo(QObject* parent, Robot* slave, qreal maxErrorD = 100., qreal maxErrorA = 10 * M_PI/180., qreal bAngle = 0., Point bPoint = Point(0.,0.), qreal threshold = 10., qreal tAngle = 0, qreal speed = 3000.);

			bool busy();
			void step();

			qreal getSpeed();

		protected:
			qreal speed;
			qreal bAngle; //base angle
			Point bPoint; //base point
			qreal threshold;
			qreal tAngle; //target angle
			qreal maxErrorD;
			qreal maxErrorA;

		private:
			Point tPoint; //target point 
		};
	}
}

#endif // DRIVETO_H
