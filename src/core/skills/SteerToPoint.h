#pragma once
#ifndef STEERTOPOINT_H
#define STEERTOPOINT_H

#include "LibIntelligence.h"
#include "Steer.h"
#include "Stage.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class SteerToPoint : public Steer
		{
		public:
			SteerToPoint(QObject* parent, Robot* slave, Point *point, Vector speed=Vector());

			void step();

		protected:
			Point *point_;
		};

		class SteerToBall : public SteerToPoint
		{
		public:
			SteerToBall(QObject* parent, Robot* slave, Vector speed=Vector())
				: SteerToPoint(parent, slave, NULL, speed) {point_ = (Point *)stage()->ball();}

			SteerToBall(QObject* parent, Robot* slave, qreal speedX, qreal speedY)
				: SteerToPoint(parent, slave, NULL, Vector(speedX, speedY)) {point_ = (Point *)stage()->ball();}
		};
	}
}

#endif // STEERTOPOINT_H
