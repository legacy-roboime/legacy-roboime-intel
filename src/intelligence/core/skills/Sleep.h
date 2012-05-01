#pragma once
#ifndef SLEEP_H
#define SLEEP_H

#include "LibIntelligence.h"
#include "Skill.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class Sleep : public Skill
		{
			Q_OBJECT

		public:
			Sleep(QObject *parent, Robot* slave);

			void step();
		};
	}
}

#endif // SLEEP_H
