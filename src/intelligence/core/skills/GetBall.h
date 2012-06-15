#pragma once
#ifndef GETBALL_H
#define GETBALL_H

#include "LibIntelligence.h"
#include "Goto.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class GetBall : public Goto //BROKEN CODE
		{
			Q_OBJECT

		public:
			GetBall(QObject* parent, Robot* slave, qreal speed, qreal orientation=0.0);
			bool hasBall(); 
			void step();
			qreal getSpeed();

		private:
			qreal speed;
		};
	}
}

#endif // GETBALL_H
