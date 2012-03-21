#pragma once
#ifndef FORCEFIELDMOTION_H
#define FORCEFIELDMOTION_H

#include "LibIntelligence.h"
#include "Steer.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class ForceFieldMotion : public Steer
		{
			Q_OBJECT

		public:
			ForceFieldMotion(QObject* parent, Robot* slave, qreal targetX=0.0, qreal targetY=0.0, qreal orientation=0.0, qreal speed=0.0, qreal repulsion=1.0, qreal attraction=1.0);

			void step();
			void setAll(qreal targetX, qreal targetY, qreal speed, qreal repulsion, qreal attraction);
			void setPoint(qreal targetX, qreal targetY);
			void setSpeed(qreal speed);
			void setRepulsion(qreal repulsion);
			void setAttraction(qreal attraction);
			void setIgnoreBall();
			void setNotIgnoreBall();
			void setIgnoreEnemy();
			void setNotIgnoreEnemy();
			void setIgnoreFriend();
			void setNotIgnoreFriend();

		protected:
			qreal targetX, targetY, speed, repulsion, attraction;
			bool ignoreBall;
			bool ignoreEnemy;
			bool ignoreFriend;
		};
	}
}

#endif // FORCEFIELDMOTION_H
