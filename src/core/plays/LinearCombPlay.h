#ifndef LINEAR_COMB_PLAY_H
#define LINEAR_COMB_PLAY_H

#include "LibIntelligence.h"
#include "LinearCombTactic.h"
#include "Plays.h"
#include "Tactics.h"

namespace LibIntelligence
{
	namespace Plays
	{
		
		class LinearCombPlay : public Play
		{
			Q_OBJECT

		public:
			LinearCombPlay(QObject *parent, Team* team ,Stage* stage);
			void setCoefs( qreal *c0, qreal *c1, qreal *c2, qreal *c3, qreal *c4 );
			void step();
		};
	}
}

#endif // LINEAR_COMB_PLAY_H