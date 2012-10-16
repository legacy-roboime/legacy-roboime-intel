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

			virtual void step() = 0;
			virtual bool busy() = 0;
			virtual void setSpeed(qreal){};

			void setRobot(Robot*);
			const Robot* robot() const;
			const Stage* stage() const;

		protected:
			Robot* robot();
			Stage* stage();

		private:
			Robot* robot_;
		};
	}
}

#endif // SKILL_H
