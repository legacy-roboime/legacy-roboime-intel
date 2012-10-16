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
			DriveToObject(QObject* parent, Robot* slave, const Object* object, qreal radiusObj, const Object* refLookPoint, qreal maxErrorD = 100., qreal maxErrorA = 10 * M_PI/180., qreal speed = 3000., bool deterministic = true, qreal maxAngVar = 15.);
			~DriveToObject(void);

			void step();
			bool busy(); 

			const Object* getRefLookPoint() const;
			void setRefLookPoint(const Object* refLookPoint);

			const Object* getLookPoint() const;
			//void setLookPoint(Object* lookPoint);

			void setObject(const Object* object);

		private:
			const Object* refLookPoint_;//can not modif
			Object* lookPoint; //can modif
			const Object* object;
			qreal maxAngVar_;
		};
	}
}

#endif // DRIVETOOBJECT_H
