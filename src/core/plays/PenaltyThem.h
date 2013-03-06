#pragma once
#ifndef PENALTYTHEM_H
#define PENALTYTHEM_H

#include "LibIntelligence.h"
#include <QObject>
#include "Play.h"
#include "Tactics.h"

namespace LibIntelligence
{
	namespace Plays{

		class PenaltyThem : public Play
		{
			Q_OBJECT

		public:
			PenaltyThem(QObject*, Team*,Stage*, Robot* gk);
			~PenaltyThem();
			virtual void step();
            void setGoalkeeper(Robot* gk);

		private:
			QVector<Goto*> gotos;
		};
	}
}

#endif // PENALTYTHEM_H

