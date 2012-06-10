#pragma once
#ifndef SAMPLEDKICK_H
#define SAMPLEDKICK_H

#include "LibIntelligence.h"
#include "DriveToBall.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class SampledKick : public DriveToBall
		{
			Q_OBJECT

		public:
			SampledKick(QObject* parent, Robot* slave, Object* lookPoint, bool deterministic = true, qreal minPower = 0., qreal maxPower = 1., qreal speed = 3000., bool pass = false);

			void step();
			virtual bool busy();
			qreal calculatePassPower(qreal s);
			void setPowerK(qreal);

		protected:


		private:
			qreal minPower_;
			qreal maxPower_;
			qreal powerK;
			bool pass_;
		};
	}
}

#endif // SAMPLEDKICK_H
