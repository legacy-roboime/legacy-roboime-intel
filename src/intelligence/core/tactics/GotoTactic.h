#pragma once

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Skills.h"

using namespace LibIntelligence::Skills;

namespace LibIntelligence
{
	namespace Tactics
	{
		class GotoTactic : public Tactic
		{
			Q_OBJECT

		public:
			GotoTactic(QObject *p, Robot* r);
			~GotoTactic();

			void step();

			Goto* goto_;
		};
	}	
}