#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "LibIntelligence.h"

namespace LibIntelligence
{
	class Object
	{
	public:
		Object(qreal x=-10000.0, qreal y=-10000.0, qreal z=0.0, qreal speedX=0.0, qreal speedY=0.0, qreal speedZ=0.0);
		Object(const Object& object);

		void setX(qreal);
		void setX(qreal, qreal);
		qreal x() const;

		void setY(qreal);
		void setY(qreal, qreal);
		qreal y() const;

		void setZ(qreal);
		void setZ(qreal, qreal);
		qreal z() const;

		//TODO: implement speed estimation
		void setSpeedX(qreal);
		qreal speedX() const;

		void setSpeedY(qreal);
		qreal speedY() const;

		void setSpeedZ(qreal);
		qreal speedZ() const;

		//syntatic sugars:
		void setXY(qreal, qreal);
		void setXYZ(qreal, qreal, qreal);
		void setSpeedXY(qreal, qreal);
		void setSpeedXYZ(qreal, qreal, qreal);
		
		Object& operator=(const Object& object);
	protected:
		qreal x_, y_, z_, speedX_, speedY_, speedZ_;
		//TODO: colocar velocidades angulares
	};
}

#endif // OBJECT_H
