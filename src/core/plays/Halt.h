#pragma once
#include "LibIntelligence.h"
#include <list>
#include <string.h>
#include "Play.h"

using namespace std;

namespace LibIntelligence
{
	namespace Plays
	{	
		class Halt : public Play
		{
			Q_OBJECT

		public:
			Halt(QObject*, Team*,Stage*);

		protected:
			void step();

			Skills::Skill *wait_[5];
		};
	}
}