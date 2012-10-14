#pragma once
#ifndef DEFENDER_H
#define DEFENDER_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "Skills.h"
#include "MachineTransition.h"

namespace LibIntelligence
{
	namespace Tactics
	{
		class Defender : public Tactic
		{
			Q_OBJECT

		public:
			Defender(QObject* parent, Robot* slave, Object* enemy, Point* cover, qreal dist, qreal speed);
			
			void setEnemy(Object* enemy);
			Object* enemy();
			
			void setCover(Point* cover);
			void follow();

		protected:
			Object* enemy_;
			Skills::DriveToObject* driveToObj;
			Skills::FollowAndCover* fac;
		};
	}	

	namespace DefenderT
	{
		class DriveObjToFacT : public MachineTransition{ 
		public:
			DriveObjToFacT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};

		class FacToDriveObjT : public MachineTransition{ 
		public:
			FacToDriveObjT(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
			bool condition();
		};
	}
}

#endif // DEFENDER_H