#pragma once
#ifndef DRIVETOBALL_H
#define DRIVETOBALL_H

#include "LibIntelligence.h"
#include "DriveToObject.h"
#include "Object.h"
#include "GotoAvoid.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class DriveToBall : public DriveToObject
		{
			Q_OBJECT

		public:
			DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed = 3000., bool deterministic = true, qreal maxAngVar = 15., qreal maxErrorD = 100., qreal maxErrorA = 10 * M_PI/180.);
			~DriveToBall(void);

			void step();
			bool busy(); 

		private:
			bool inCone;
			GotoAvoid* gotoa;
		};
	}
}

#endif // DRIVETOBALL_H
