#pragma once
#ifndef DRIVETOOBJECT_H
#define DRIVETOOBJECT_H

#include "LibIntelligence.h"
#include "DriveTo.h"
#include "Object.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class DriveToObject : public DriveTo
		{
			Q_OBJECT

		public:
			DriveToObject(QObject* parent, Robot* slave, const Object* object, const Object* refLookPoint, qreal speed = 3000., bool deterministic = true, qreal maxAngVar = 15.);
			~DriveToObject(void);

			void step();
			bool busy(); 

			const Object* getRefLookPoint() const;
			void setRefLookPoint(const Object* refLookPoint);

			const Object* getLookPoint() const;
			//void setLookPoint(Object* lookPoint);

		private:
			const Object* refLookPoint_;//can not modif
			Object* lookPoint; //can modif
			const Object* object;
			qreal maxAngVar_;
		};
	}
}

#endif // DRIVETOOBJECT_H
