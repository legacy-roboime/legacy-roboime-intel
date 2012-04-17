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
			SampledKick::~SampledKick(void); 
			void step();
			virtual bool busy();

		protected:


		private:
			qreal minPower_;
			qreal maxPower_;
			bool pass_;
		};
	}
}

#endif // SAMPLEDKICK_H
