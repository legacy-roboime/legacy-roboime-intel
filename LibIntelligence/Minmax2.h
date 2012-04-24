#pragma once
#include <deque>
#include <QVector>
#include <limits>
#include "Plays.h"
#include "Skills.h"
#include "Stage.h"
#include <QFile>
#include <QDataStream>

#ifdef __cplusplus
extern "C" {
#endif

#include "soccer.h"

#ifdef __cplusplus
}

#endif

#define MAX_NPLAYS 2//15//50
#define MIN_NPLAYS 2//15//50

#define MINIMAX_MAX_LEVEL 2

using namespace LibIntelligence::Skills;

namespace LibIntelligence
{
	namespace Plays
	{
		class Minmax2: public Play {

		private:
			QFile log; 

			QVector<Goto*> _max_skills;
			SoccerState *s;
			SoccerAction best_action;
			int depth_;
			float alpha_;
			float beta_;
			qreal speed_;

			SoccerAction* minimax_get_best_action( void );
			float minimax_getMaxValue( SoccerState s, int depth, float alpha, float beta );
			float minimax_getMinValue( SoccerState s, int depth, float alpha, float beta );
			SoccerAction minimax_expandMax( SoccerState *s, int i, int depth );
			void minimax_expandMin( SoccerState *s, int i, int depth );
			void saction_act();
			void update_soccer_state();
			int ballOwner(bool us);

		public:
			Minmax2(QObject *parent, Team* team ,Stage* stage, int depth = MINIMAX_MAX_LEVEL, float alpha = -MAX_FLOAT, float beta = MAX_FLOAT, qreal speed = 3000.);
			~Minmax2(); 
			void step();
		};
	}
}