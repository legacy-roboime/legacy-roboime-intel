#pragma once
#ifndef ZICKLER43_H
#define ZICKLER43_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "DriveToBall.h"
#include "SampledDribble.h"
#include "SampledKick.h"

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
			

		protected:
			Skills::DriveToBall* driveToBall_; //pegar a bola de forma a preparar o chute pro gol
			Skills::SampledDribble* sampledDribble_; //conduzir com potencia do driblador aleatoria e na direcao do gol adversario com um pequeno erro na direcao 
			Skills::SampledKick* sampledGoalKick_; //chutar a bola pro gol com potencia proxima da maxima
			Skills::SampledKick* sampledMiniKick_; //chutar a bola com potencia proxima da minima e na direcao do gol adversario com um pequeno erro na direcao
			Skills::Wait* wait_;

			qreal speed;

		private:
			Q_INVOKABLE bool driveToDribble();
			//Q_INVOKABLE bool dribbleToDrive();
			Q_INVOKABLE bool dribbleToGoalKick();
			Q_INVOKABLE bool dribbleToMiniKick();
			Q_INVOKABLE bool dribbleToDribble();
			Q_INVOKABLE bool defaultTrue();
		};
	}	
}

#endif // ZICKLER43_H

