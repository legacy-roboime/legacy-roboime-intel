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

		virtual ~Object() {};

		void updatePosition(const Point &);
		void updateSpeed(double time);

		void setOrientation(qreal);
		void setOrientationWithFilter(qreal);
		qreal orientation() const;

		//TODO: implement speed estimation
		void setSpeed(const Vector &);
		void setSpeed(const qreal &sx, const qreal &sy);
		Vector speed() const;

		void setAngSpeed(const qreal &);
		qreal angSpeed() const;

		//bool useFilter() { return useFilter_; }
		//void setUseFilter(bool b) { useFilter_ = b; }
		void updatePositionWithFilter(const Point &);

	private:
		Vector speed_;
		qreal theta_, omega_;

		qreal timeOld_, thetaOld_;
		Point posOld_;

		//low pass filter stuff
		//TODO: make this a filter object??
		//XXX: only works for 120hz sampling rate
		//bool useFilter_;
		float ux[4], vx[4], uy[4], vy[4], uo[4], vo[4];
		static float gain, coef[4];
	};
}

#endif // OBJECT_H