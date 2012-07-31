#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "LibIntelligence.h"
#include <QVector2D>
#include <QPointF>
#include <qline.h>
#include <qqueue.h>
#include "LinearRegression.h"

namespace LibIntelligence
{
	class Object : public QPointF
	{
	public:
		Object(qreal x=-10000.0, qreal y=-10000.0, qreal speedX=0.0, qreal speedY=0.0, qreal orientation = 0.0, qreal angSpeed = 0.0);
		Object(const QPointF &center, const QVector2D &speed, const qreal &theta, const qreal &omega);
		Object(const Object& object);

		void updatePosition(const QPointF &);
		void updateSpeed(double time);

		void setOrientation(qreal);
		qreal orientation() const;

		//TODO: implement speed estimation
		void setSpeed(const QVector2D &);
		void setSpeed(const qreal &sx, const qreal &sy);
		QVector2D speed() const;

		void setAngSpeed(const qreal &);
		qreal angSpeed() const;

	private:
		QVector2D speed_;
		qreal theta_, omega_;

		qreal timeOld_, thetaOld_;
		QPointF posOld_;
	};
}

#endif // OBJECT_H