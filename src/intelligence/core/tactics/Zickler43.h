#pragma once
#ifndef ZICKLER43_H
#define ZICKLER43_H

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
		class Zickler43 : public Tactic
		{
			Q_OBJECT

		public:
			Zickler43(QObject* parent, Robot* slave, qreal speed=3000, bool deterministic = false);
			//Zickler43(QObject* p, Robot* r, const Zickler43& zickler);
			~Zickler43();
			void step();
			void setSpeed(qreal speed);

		protected:
			Skills::DriveToBall* driveToBall_; //pegar a bola de forma a preparar o chute pro gol
			Skills::SampledDribble* sampledDribble_; //conduzir com potencia do driblador aleatoria e na direcao do gol adversario com um pequeno erro na direcao 
			Skills::SampledKick* sampledGoalKick_; //chutar a bola pro gol com potencia proxima da maxima
			Skills::SampledKick* sampledMiniKick_; //chutar a bola com potencia proxima da minima e na direcao do gol adversario com um pequeno erro na direcao
			Skills::Wait* wait_;

		private:
			Object* lookPoint;
			void updateLookPoint();
			Point pointToKick();
			bool isKickScored( Point p );
		};
	}	

	namespace Zickler43T
	{
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

		class GoalKickToDriveT : public MachineTransition{ 
		public:
			GoalKickToDriveT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DribbleToMiniKickT : public MachineTransition{ 
		public:
			DribbleToMiniKickT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DribbleToDribbleT : public MachineTransition{ 
		public:
			DribbleToDribbleT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class DefaultTrueT : public MachineTransition{ 
		public:
			DefaultTrueT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};
	}
}

#endif // ZICKLER43_H

