#pragma once
#include <QQueue>
#include <limits>
#include "Tactics.h"
#include "Play.h"
#include "Stage.h"
#include <QFile>
#include "tree.hh"
#include "Goto.h"
#include <QTimer>
#include "StageY.h"

using namespace LibIntelligence::Tactics;
using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

namespace LibIntelligence
{
	namespace Plays
	{
		class BGT: public Play {
			Q_OBJECT

		private:
			QFile planningLog;
			QFile executeLog;
			QTimer* timer;

			QQueue<Goto*> execSkills;
			StageY* xInit;
			tree<StageY*>* tree_;
			unsigned int maxIter;
			unsigned int planSteps;
			unsigned int execSteps;
			unsigned long debugTime;
			qreal mi_;
			bool planningInterruped_;

		private slots:
				void planningInterruped();

		public:
			BGT(QObject *parent, Team* myTeam, Stage* stage);
			~BGT();
			void step();
			StageY* selectNodeBGT(qreal mi);
			StageY* tacticsDrivenPropagate(const StageY& stage);
			void rollBack(StageY* stage);
			void traceBack(StageY* stage, QQueue<StageY*>* solution);
			bool validate(StageY* stage);
			bool isGoal(StageY* stage);
			qreal evalState(StageY* stage);
			void planning(QQueue<StageY*>* solution);
			void goToStage(StageY* stage);
			void populateTactics(const StageY* source, StageY* target);
			qreal averageLeafDepth();
			qreal averageBranchingFactor();
			StageY* randomLeaf(); //not include busy stages
			StageY* randomNonLeaf(); //not include busy stages
		};
	}
}