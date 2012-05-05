#pragma once
#ifndef ATTACKERMINMAX2_H
#define ATTACKERMINMAX2_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "DriveToBall.h"
#include "SampledDribble.h"
#include "SampledKick.h"
#include "MachineTransition.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class AttackerMinMax2 : public Tactic
		{
			Q_OBJECT

		public:
			AttackerMinMax2(QObject* parent, Robot* slave, qreal speed = 1000., qreal dribbleSpeed = 500., qreal passSpeed = 800.);
			//AttackerMinMax2(QObject* p, Robot* r, const AttackerMinMax2& zickler);
			~AttackerMinMax2();
			Object* kickPoint();
			Object* movePoint();
			bool hasKick();
			bool hasPass();
			void updateSoccerAction(bool hasKick = false, bool hasPass = false, bool getBall = false, qreal kickPointX = 0, qreal kickPointY = 0, qreal movePointX = 0, qreal movePointY = 0);
			bool getMinDist(); //se o robo atingiu uma distância mínima para desligar o move do minimax2 e usar skills para pegar a bola
			Skills::SampledDribble* dribble();
			qreal minDist();

		protected:
			//Skills::DriveToBall* driveToBall_;
			Skills::SampledDribble* dribble_;
			Skills::SampledKick* goalKick_;
			Skills::SampledKick* pass_;
			Skills::Goto* goto_;

			qreal speed;

		private:
			qreal minDist_;
			Object* kickPoint_;
			bool hasKick_;
			bool hasPass_;
			Object* movePoint_;
			Object* dribblePoint_;
		};
	}	

	namespace AttackerMinMax2T
	{
		class GotoToDriveT : public MachineTransition{ 
		public:
			GotoToDriveT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DriveToGotoT : public MachineTransition{ 
		public:
			DriveToGotoT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DriveToDribbleT : public MachineTransition{ 
		public:
			DriveToDribbleT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DribbleToDriveT : public MachineTransition{ 
		public:
			DribbleToDriveT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DribbleToGoalKickT : public MachineTransition{ 
		public:
			DribbleToGoalKickT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class GoalKickToDribbleT : public MachineTransition{ 
		public:
			GoalKickToDribbleT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DribbleToPassT : public MachineTransition{ 
		public:
			DribbleToPassT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class PassToDribbleT : public MachineTransition{ 
		public:
			PassToDribbleT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DefaultTrueT : public MachineTransition{ 
		public:
			DefaultTrueT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};
	}
}

#endif // ATTACKERMINMAX2_H

