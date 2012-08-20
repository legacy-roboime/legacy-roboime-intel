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
			DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed = 3000., bool deterministic = true, qreal maxAngVar = 15., qreal maxErrorD = 150., qreal maxErrorA = 30 * M_PI/180.);
			~DriveToBall(void);

			void step();
			bool busy(); 

		private:
			bool inCone;
			GotoAvoid* gotoa; //ARQUITETURA NAO BOA, POIS AO DAR SETROBOT EM DRIVETOBALL TERA QUE DAR SET ROBOT NESSA SKILL
		};
	}
}

#endif // DRIVETOBALL_H
