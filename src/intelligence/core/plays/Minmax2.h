#pragma once
#include <deque>
#include <QVector>
#include <limits>
#include "Plays.h"
#include "Skills.h"
#include "Tactics.h"
#include "Stage.h"
#include <QFile>
#include <QDataStream>

#include "soccer.h"

#define MAX_NPLAYS 23//23//15//50
#define MIN_NPLAYS 23//23//15//50

#define MINIMAX_MAX_LEVEL 4

using namespace LibIntelligence::Tactics;
using namespace LibIntelligence::Skills;

namespace LibIntelligence
{
	namespace Plays
	{
		class Minmax2: public Play {

		private:
			QFile log; 
			int teste;

			AttackerMinMax2* attacker;
			QVector<Goto*> _max_skills;
			SoccerState *s;
			SoccerAction best_action;
			SoccerAction prev_best_action;
			int depth_;
			float alpha_;
			float beta_;
			qreal speed_;

			SoccerAction* minimax_get_best_action( void );
			float minimax_getMaxValue( SoccerState s, int depth, float alpha, float beta );
			float minimax_getMinValue( SoccerState s, int depth, float alpha, float beta );
			SoccerAction minimax_expandMax( SoccerState *s, int i, int depth );
			void minimax_expandMin( SoccerState *s, int i, int depth );
			void act();
			void saction_act( SoccerState *s, SoccerAction *sa );
			void update_soccer_state();
			void ballOwner();

		public:
			Minmax2(QObject *parent, Team* team ,Stage* stage, int depth = MINIMAX_MAX_LEVEL, float alpha = -MAX_FLOAT, float beta = MAX_FLOAT, qreal speed = 3000.);
			~Minmax2(); 
			void step();
		};
	}
}