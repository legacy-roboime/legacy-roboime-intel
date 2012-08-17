#ifndef GOTOAVOID_H
#define GOTOAVOID_H

#include "Goto.h"
#include "Stage.h"
#include "Ball.h"
#include "PID.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class GotoAvoid : public Goto
		{
		public:
			GotoAvoid(QObject *parent, Robot *slave, Point *target, Point *avoid, qreal radiusAvoid, qreal speed);
			GotoAvoid(QObject *parent, Robot *slave, Point target, Point *avoid, qreal radiusAvoid, qreal speed);
			~GotoAvoid();

			void setTarget(qreal x, qreal y);
			void setTarget(Point *);
			void setAvoid(Point *);
			void setSpeed(qreal);
			Point* getAvoid();
			qreal getRadiusAvoid();

			void step();
			bool busy(); 

		protected:
			qreal radiusAvoid;
			Point *avoid;
			Point *tangPoint;
			CONTROLLER_S circlePID;
		};

		class GotoBall : public GotoAvoid
		{
		public:
			GotoBall(QObject *parent, Robot *slave, Point *lookPoint, qreal radiusAvoid, qreal speed)
				: GotoAvoid(parent, slave, Point(), NULL, radiusAvoid, speed), lookPoint(lookPoint) {
					setAvoid(stage()->ball());
			}

			void step() {
				Line line(*lookPoint, *avoid);
				line.setLength(line.length() + robot()->body().radius() + stage()->ball()->radius());
				target = line.p2();
				GotoAvoid::step();
			}

		protected:
			Point *lookPoint;
		};
	}
}

#endif