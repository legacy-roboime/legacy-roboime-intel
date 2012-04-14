#pragma once
#ifndef DRIVETO_H
#define DRIVETO_H

#include "LibIntelligence.h"
#include "Goto.h"
#include "Wait.h"
#include <QPointF>

namespace LibIntelligence
{
	namespace Skills
	{
		class DriveTo : public Goto
		{
			Q_OBJECT

		public:
			DriveTo(QObject* parent, Robot* slave, qreal bAngle = 0., QPointF bPoint = QPointF(0.,0.), qreal threshold = 10., qreal tAngle = 0, qreal speed = 3000.);
			DriveTo::~DriveTo(void);

			bool busy(); 
			void step();

			qreal getSpeed();

		protected:
			qreal speed;
			qreal bAngle; //base angle
			QPointF bPoint; //base point
			qreal threshold;
			qreal tAngle; //target angle

		private:
			QPointF tPoint; //target point 
		};
	}
}

#endif // DRIVETO_H
