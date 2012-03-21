#pragma once
#ifndef MOVE_H
#define MOVE_H

#include "LibIntelligence.h"
#include "Skill.h"
#include <QTime>

#define ROBOT_MAX_LINEAR_SPEED 1000
#define ROBOT_MAX_ANGULAR_SPEED 15;


namespace LibIntelligence
{
	namespace Skills
	{
		class Move : public Skill
		{
			Q_OBJECT

		public:
			Move(QObject* parent, Robot* slave, qreal speedX=0.0, qreal speedY=0.0, qreal speedAngular=0.0);

			void step();
			void setAll(qreal speedX, qreal speedY, qreal speedAngular);
			void setSpeeds(qreal speedX, qreal speedY);
			void setSpeedAngular(qreal speedAngular);
			bool busy();

		private:
			qreal speedX, speedY, speedAngular;
			qreal oldSpeedX, oldSpeedY, oldSpeedAngular;
			QTime oldTime;
		};
	}
}

#endif // MOVE_H
