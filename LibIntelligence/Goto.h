#pragma once
#ifndef GOTO_H
#define GOTO_H

#include "LibIntelligence.h"
#include "ForceFieldMotion.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class Goto : public ForceFieldMotion
		{
			Q_OBJECT

		public:
			Goto(QObject* parent, Robot* slave, qreal x=0.0, qreal y=0.0, qreal orientation=0.0, qreal speed=0.0, bool allowDefenseArea=false);

			void step();
			void setAll(qreal x, qreal y, qreal speed);
			void setIgnoreBrake();
			void setNotIgnoreBrake();
			void setAllowDefenseArea();
			void setNotAllowDefenseArea();

		protected:
			bool ignoreBrake;
			bool allowDefenseArea;
		};
	}
}

#endif // GOTO_H
