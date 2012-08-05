#pragma once
#ifndef PENALTYUS_H
#define PENALTYUS_H

#include "LibIntelligence.h"
#include <QObject>
#include "Play.h"
#include "Tactics.h"

namespace LibIntelligence
{
	namespace Plays{

		class PenaltyUs : public Play
		{
			Q_OBJECT

		public:
			PenaltyUs(QObject*, Team*,Stage*, Robot* penaltyKicker, Robot* goalkeeper);
			~PenaltyUs();
			virtual void step();

		private:
			QVector<Goto*> gotos;
		};
	}
}

#endif // PENALTYUS_H
