#pragma once
#ifndef STEERTOBALL_H
#define STEERTOBALL_H

#include "LibIntelligence.h"
#include "Steer.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class SteerToBall : public Steer
		{
			Q_OBJECT

		public:
			SteerToBall(QObject* parent, Robot* slave, qreal speedX=0.0, qreal speedY=0.0);

			void step();
		};
	}
}

#endif // STEERTOBALL_H
