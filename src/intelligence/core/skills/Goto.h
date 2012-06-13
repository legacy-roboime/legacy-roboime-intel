#pragma once
#ifndef GOTO_H
#define GOTO_H

#include "LibIntelligence.h"
#include "PID.h"
#include "Steer.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class Goto : public Steer
		{
			Q_OBJECT

		public:
			Goto(QObject* parent, Robot* slave, qreal x=0.0, qreal y=0.0, qreal orientation=0.0, qreal speed=0.0, bool allowDefenseArea=false);
			void step();
			void setAll(qreal x, qreal y, qreal speed);
			void setIgnoreBrake();
			void setNotIgnoreBrake();
			void setAllowDefenseArea();
			void setNotAllowDefenseArea();
			void setSpeed(qreal);
			void setPIDkGoto(double, double, double);
			void printPIDk();
			void setPoint(const QPointF &);
			void setPoint(qreal targetX, qreal targetY);

		protected:
			bool ignoreBrake;
			bool allowDefenseArea;
			CONTROLLER_S controllerSpeedX;
			CONTROLLER_S controllerSpeedY;
			qreal speed, targetX, targetY;
		};
	}
}

#endif // GOTO_H
