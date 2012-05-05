#pragma once
#ifndef STAGEX_H
#define STAGEX_H

#include "LibIntelligence.h"
#include "Stage.h"
#include "StageY.h"
#include "UpdaterSIMi.h"
#include <QVector>

class Simulation;

namespace LibIntelligence
{
	class StageX : public StageY
	{
		Q_OBJECT

	public:
		StageX(const Stage& stage);
		//StageX(const StageY& stage);
		~StageX();
		
		void simulate(const qreal timeStep = 1./60.);
		void releaseScene();
		void build();
		uint getSceneNumber();

	private:
		friend class Simulation;

		static bool init;
		static Simulation* /*const*/ sim;
		static const int MAX_NUMBER_SCENES = 1;
		static qreal defaultTimeStep; //TODO: realizar um set na classe Simulation para mudar o timeStep para esse valor 
		static bool busyScenes[MAX_NUMBER_SCENES];
		uint sceneNumber;
		bool built;
		UpdaterSIMi* updater;
	public:
		QVector<QVector<float>>* lastWheelSpeed;
		QVector<QVector<float>>* lastDesiredWheelSpeed;
		QVector<QVector<float>>* lastWheelTorque;
	};
}

#endif // STAGEX_H
