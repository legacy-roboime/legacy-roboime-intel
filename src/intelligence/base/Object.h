#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "LibIntelligence.h"
#include <QQueue>
#include "geomutils.h"
#include "LinearRegression.h"

namespace LibIntelligence
{
	class Object : public Point
	{
	public:
		Object(qreal x=-10000.0, qreal y=-10000.0, qreal speedX=0.0, qreal speedY=0.0, qreal orientation = 0.0, qreal angSpeed = 0.0);
		Object(const Point &center, const Vector &speed, const qreal &theta, const qreal &omega);
		Object(const Object& object);

		void updatePosition(const Point &);
		void updateSpeed(double time);

		void setOrientation(qreal);
		qreal orientation() const;

		//TODO: implement speed estimation
		void setSpeed(const Vector &);
		void setSpeed(const qreal &sx, const qreal &sy);
		Vector speed() const;

		void setAngSpeed(const qreal &);
		qreal angSpeed() const;

	private:
		Vector speed_;
		qreal theta_, omega_;

		qreal timeOld_, thetaOld_;
		Point posOld_;
	};
}

#endif // OBJECT_H