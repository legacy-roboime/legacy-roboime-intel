#pragma once
#ifndef LOOPS_H
#define LOOPS_H

#include "LibIntelligence.h"
#include "Skill.h"

namespace LibIntelligence
{
	namespace Skills
	{
		namespace Loops
		{
			class Orbit : public Goto
			{
			public:
				Orbit(QObject* parent, Robot* slave, qreal x, qreal y, qreal radius, qreal speedLinear, qreal speedAngular);
				
				void setAll(qreal x, qreal y, qreal radius, qreal speedLinear, qreal speedAngular);
				void step();

			private:
				qreal centerX, centerY, radius, speedLinear, speedAngular;
			};

			class Circle : public Orbit
			{
			public:
				Circle(QObject* parent, Robot* slave, qreal x, qreal y, qreal radius, qreal speedLinear);
			};
		}
	}
}

#endif // LOOPS_H