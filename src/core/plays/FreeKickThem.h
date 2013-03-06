#pragma once
#ifndef FREEKICKTHEM_H
#define FREEKICKTHEM_H

#include "LibIntelligence.h"
#include <QObject>
#include "Play.h"
#include "Tactics.h"

namespace LibIntelligence
{
	namespace Plays{

		class FreeKickThem : public Play
		{
			Q_OBJECT

		public:
			FreeKickThem(QObject*, Team*,Stage*);
			~FreeKickThem();
			virtual void step();

		private:
			QPointF lastBall;
		};
	}
}

#endif // FREEKICKTHEM_H
