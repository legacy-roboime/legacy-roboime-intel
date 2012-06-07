#pragma once
#include <deque>
#include <QVector>
#include <limits>
#include "Play.h"
#include "Skills.h"
#include "Tactics.h"
#include "Stage.h"
#include <QFile>
#include <QDataStream>

#define SOCCER_DEBUG

using namespace LibIntelligence::Tactics;
using namespace LibIntelligence::Skills;

namespace LibIntelligence
{
	namespace Plays
	{
		class Minmax2: public Play {

		private:
			QFile log; 

			AttackerMinMax2* attacker;
			QVector<Goto*> _max_skills;
			//Goto* goto_;
			qreal speed_;
			bool usRed;

			SoccerState *s;
			int depth_;
			float alpha_;
			float beta_;
			SoccerAction red_action, blue_action;
			bool init;
			SoccerEnvironment envReal;

			void act(SoccerAction& action);
			void saction_act( SoccerState *s, SoccerAction *sa );
			void update_soccer_state();
			void ballOwner();
			void changeSEnvMeasure(SoccerEnvironment *s, double scale);
			void changeSStateMeasure(SoccerState *s, double scale);
			void changeSActionMeasure(SoccerAction *a, double scale);

		public:
			Minmax2(QObject *parent, Team* team ,Stage* stage, int depth = MINIMAX_MAX_LEVEL, float alpha = -MAX_FLOAT, float beta = MAX_FLOAT, qreal speed = 3000.);
			~Minmax2(); 
			void step();
		};
	}
}