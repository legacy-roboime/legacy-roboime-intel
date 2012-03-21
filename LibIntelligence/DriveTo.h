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
			DriveTo(QObject* parent, Robot* slave, qreal bAngle = 0., QPointF bPoint = QPointF(0.,0.), qreal threshold = 10., qreal speed = 3000.);
			DriveTo::~DriveTo(void);
			bool busy(); 
			void step();
			qreal getSpeed();

		protected:
			qreal speed;
			qreal bAngle; //base angle
			QPointF bPoint; //base point
			qreal threshold;
			Wait* wait;

		private:
			qreal tAngle; //target angle
			QPointF tPoint; //target point 
			//TODO: deixar o tAngle flexivel pois dessa forma vai da pra implementar os blockers tb
		};
	}
}

#endif // DRIVETO_H
