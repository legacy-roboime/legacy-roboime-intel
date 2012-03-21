#pragma once
#ifndef WAIT_H
#define WAIT_H

#include "LibIntelligence.h"
#include "Skill.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class Wait : public Skill
		{
			Q_OBJECT

		public:
			Wait(QObject* parent, Robot* slave);
			bool busy();

			void step();
		};
	}
}

#endif // WAIT_H
