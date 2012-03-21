#pragma once
#ifndef DRIVETOBALL_H
#define DRIVETOBALL_H

#include "LibIntelligence.h"
#include "DriveTo.h"
#include "Object.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class DriveToBall : public DriveTo
		{
			Q_OBJECT

		public:
			DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed = 3000.);
			~DriveToBall(void);
			void step();
			void setRefLookPoint(const Object* refLookPoint);
			bool busy(); 

		protected:
			const Object* refLookPoint_;//can not modif
			Object lookPoint; //can modif
		};
	}
}

#endif // DRIVETOBALL_H
