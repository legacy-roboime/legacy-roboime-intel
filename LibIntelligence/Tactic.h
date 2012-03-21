#pragma once
#ifndef TACTIC_H
#define TACTIC_H

#include "LibIntelligence.h"
#include <QObject>
#include <QVector>
#include "StateMachine.h"

namespace LibIntelligence
{
	class Robot;
	class Stage;
	
	namespace Skills
	{
		class Skill;
	}

	namespace Tactics
	{
		class Tactic : public StateMachine
		{
		Q_OBJECT

		public:
			Tactic(QObject* parent, Robot* slave, bool deterministic = false);
			Tactic(QObject* p, Robot* s, const Tactic& tactic);
			~Tactic();

			virtual void step() = 0;

			void setStage(Stage*);
			void setRobot(Robot*);
			const Robot* robot() const;
			const Stage* stage() const;

			Robot* robot();
			Stage* stage();

		protected:

		private:
			Robot* robot_;
			Stage* stage_;
		};
	}
}

#endif // TACTIC_H
