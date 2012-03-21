#pragma once
#ifndef SKILL_H
#define SKILL_H

#include "LibIntelligence.h"
#include "State.h"
#include <QObject>
#include <iostream>

namespace LibIntelligence
{
	class Robot;
	class Stage;

	namespace Skills
	{
		class Skill : public State
		{
		Q_OBJECT

		public:
			Skill(QObject* parent, Robot* slave, bool deterministic=false);
			Skill(const Skill& skill);
			~Skill();

			virtual void step() = 0;
			virtual bool busy() = 0;

			void setRobot(Robot*);
			const Robot* robot() const;
			const Stage* stage() const;

		protected:
			Robot* robot();
			Stage* stage();

		private:
			Robot* robot_;
			//the stage is retreived from the robot
			Stage* stage_;
		};
	}
}

#endif // SKILL_H
