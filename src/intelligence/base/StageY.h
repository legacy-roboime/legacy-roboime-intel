#pragma once
#ifndef STAGEY_H
#define STAGEY_H

#include "Stage.h"
#include <QQueue>
#include "Tactic.h"

namespace LibIntelligence
{
	class StageY :	public Stage
	{
		Q_OBJECT

	public:
		StageY(const Stage& stage);
		//StageY(const StageY& stage);
		~StageY(void);
		void pushTactic(Tactics::Tactic* tactic, TeamColor color);
		bool busy(TeamColor color);
		QQueue<Tactics::Tactic*>* getBlueTactics();
		QQueue<Tactics::Tactic*>* getYellowTactics();
		//StageY& operator=(const StageY& stage);
		bool operator==(const StageY& stage);
		bool operator==(const StageY* stage);

	private:
		QQueue<Tactics::Tactic*> blueTactics; 
		QQueue<Tactics::Tactic*> yellowTactics;
	};
}
#endif // STAGE_H