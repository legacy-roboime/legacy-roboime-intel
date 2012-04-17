#pragma once
#ifndef DRIVETOBALL_H
#define DRIVETOBALL_H

#include "LibIntelligence.h"
#include "DriveToObject.h"
#include "Object.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class DriveToBall : public DriveToObject
		{
			Q_OBJECT

		public:
			DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed = 3000., bool deterministic = true);
			~DriveToBall(void);

			void step();
			bool busy(); 

		private:
		};
	}
}

#endif // DRIVETOBALL_H
