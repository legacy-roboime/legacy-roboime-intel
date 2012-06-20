#pragma once
#ifndef GOTO_H
#define GOTO_H

#include "LibIntelligence.h"
#include "PID.h"
#include "Steer.h"
#include "geomutils.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class Goto : public Steer
		{
			Q_OBJECT

		public:
			Goto(QObject* parent, Robot* slave, qreal x=0.0, qreal y=0.0, qreal orientation=0.0, qreal speed=0.0, bool allowDefenseArea=false);
			Goto(QObject* parent, Robot* slave, Point *point, qreal orientation=0.0, qreal speed=0.0, bool allowDefenseArea=false);
			Goto(QObject* parent, Robot* slave, Point *point, Point *lookAt, qreal speed=0.0, bool allowDefenseArea=false);
			Goto(QObject* parent, Robot* slave, Point point, Point *lookAt, qreal speed=0.0, bool allowDefenseArea=false);
			~Goto();

			void step(Point *target);
			void step() {step(NULL);}
			void setIgnoreBrake();
			void setNotIgnoreBrake();
			void setAllowDefenseArea();
			void setNotAllowDefenseArea();
			void setSpeed(qreal);
			void setPIDkGoto(double, double, double);
			void printPIDk();
			void setPoint(const Point &);
			void setPoint(Point *);
			void setPoint(qreal targetX, qreal targetY);
			void setLookAt(Point *);
			void setOrientation(qreal orientation);
			void setOrientation(qreal dX, qreal dY);

		protected:
			Point *pTarget;//the point where we're trying to go to
			Point target;
			Point targetCopy() {return pTarget ? *pTarget : target;}
			Point old;//where we were trying to go on the previous iteration
			Point *lookAt;//where should we be looking at, this overrides the action of the orientation
			bool ignoreBrake;//should we break when getting close to that point?
			bool allowDefenseArea;//are we allowed to enter the defense area?
			qreal speed;//how fast can we go to that point, if possible we'll try to keep that speed
			CONTROLLER_S controllerSpeedX;
			CONTROLLER_S controllerSpeedY;
		};
	}
}

#endif // GOTO_H
