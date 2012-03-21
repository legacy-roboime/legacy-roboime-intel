#pragma once
#include <deque>
#include <QVector>
#include <limits>
#include "Plays.h"
#include "Stage.h"
#include "StageX.h"
#include <QFile>
#include <QDataStream>

namespace LibIntelligence
{
	namespace Plays
	{
		class Minmax: public Play {
			Q_OBJECT

		private:
			QFile log; 

			int steps_per_turn;
			int _depth;
			int _best_action;
			QVector<Play*> _max_plays;
			QVector<Play*> _min_plays;
			Stage* _current_state;

			void expandMax( StageX *s, int action );
			void expandMin( StageX *s, int action );
			qreal evaluate( Stage* stage );
			qreal getMaxValue(StageX s, int depth );
			qreal getMinValue(StageX s, int depth );

		public:
			Minmax(QObject *parent, Team* team ,Stage* stage, int depth = 2, int best_action = 2);
			~Minmax(); 
			void step();


		};
	}
}