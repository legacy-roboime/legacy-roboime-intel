#pragma once
#ifndef WHEEL_H
#define WHEEL_H

#include "LibIntelligence.h"
#include "Actuator.h"

namespace LibIntelligence
{
	class Wheel : public Actuator
	{
		Q_OBJECT

	public:
		Wheel(const Wheel&);
		Wheel(QObject* parent=0, qreal angle=0.0, qreal radius=0.0, qreal distance=0.0, qreal speed=0.0);

		void setAngle(qreal);
		qreal angle() const;

		void setRadius(qreal);
		qreal radius() const;

		void setDistance(qreal);
		qreal distance() const;

	private:
		qreal angle_;
		qreal radius_;
		qreal distance_;
	};
}

#endif // WHEEL_H
