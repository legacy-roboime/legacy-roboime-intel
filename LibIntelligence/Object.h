#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <qline.h>
#include "LibIntelligence.h"
#include <qqueue.h>
#include <qpoint.h>
#include "LinearRegression.h"

namespace LibIntelligence
{
	class Object
	{
	public:
		Object(qreal x=-10000.0, qreal y=-10000.0, qreal z=0.0, qreal speedX=0.0, qreal speedY=0.0, qreal speedZ=0.0, qreal orientation = 0.0, qreal angSpeedZ = 0.0);
		Object(const Object& object);
		~Object();

		void setX(qreal);
		qreal x() const;

		void setY(qreal);
		void setY(qreal, qreal);
		qreal y() const;

		void setZ(qreal);
		void setZ(qreal, qreal);
		qreal z() const;

		void setOrientation(qreal);
		qreal orientation() const;

		//TODO: implement speed estimation
		void setSpeedX(qreal);
		qreal speedX() const;

		void setSpeedY(qreal);
		qreal speedY() const;

		void setSpeedZ(qreal);
		qreal speedZ() const;

		void setAngSpeedZ(qreal);
		qreal angSpeedZ() const;

		//syntatic sugars:
		void setXY(qreal, qreal);
		void setXYZ(qreal, qreal, qreal);
		void setSpeedXY(qreal, qreal);
		void setSpeedXYZ(qreal, qreal, qreal);
		
		Object& operator=(const Object& object);

		QLineF getSpeedVector();
		void updateSpeed(double time);
		Object distance(const Object* object2) const;
		qreal module() const;

	protected:
		qreal x_, y_, z_, orientation_, speedX_, speedY_, speedZ_, angSpeedZ_;
		//TODO: colocar velocidades angulares
	private:
		LinearRegression* linearRegressionVx;
		LinearRegression* linearRegressionVy;
		LinearRegression* linearRegressionVang;
		double xOld,yOld, orientationOld, timeOld;
	};
}

#endif // OBJECT_H