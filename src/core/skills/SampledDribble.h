#pragma once
#ifndef SAMPLEDDRIBBLE_H
#define SAMPLEDDRIBBLE_H

#include "LibIntelligence.h"
#include "DriveToBall.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class SampledDribble : public DriveToBall //TODO: implementar a aleatoriedade na direçao de condução da bola ver pagina 96 do zickler
		{
			Q_OBJECT

		public:
			SampledDribble(QObject* parent, Robot* slave, Object* lookPoint, bool deterministic = true, qreal minPower = 0., qreal maxPower = 1., qreal speed = 3000.);
			~SampledDribble(void);
			void step();
			bool busy();

		protected:


		private:
			qreal minPower_;
			qreal maxPower_;
		};
	}
}

#endif // SAMPLEDDRIBBLE_H
