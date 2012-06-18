#pragma once
#ifndef STEER_H
#define STEER_H

#include "LibIntelligence.h"
#include "Move.h"
#include "PID.h"
#include "geomutils.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class Steer : public Move
		{
			Q_OBJECT

		public:
			Steer(QObject* parent, Robot* slave, Vector speed, Point *lookPoint);
			Steer(QObject* parent, Robot* slave, qreal speedX=0.0, qreal speedY=0.0, qreal orientation=0.0);
			Steer(QObject* parent, Robot* slave, qreal speedX, qreal speedY, qreal dX, qreal dY);

			void step();
			void setAll(qreal speedX, qreal speedY, qreal orientation);
			void setAll(qreal speedX, qreal speedY, qreal dX, qreal dY);
			void setOrientation(qreal orientation);
			void setOrientation(qreal dX, qreal dY);
			void setLookPoint(Point *);
			void setRate(qreal);
			void setPIDk(double,double,double);

		protected:
			Point *lookPoint;
			qreal orientation, rate;

		private:
			CONTROLLER_S controller;
		};
	}
}

#endif // STEER_H
