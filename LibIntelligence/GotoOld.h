#pragma once
#ifndef GOTOOLD_H
#define GOTOOLD_H

#include "LibIntelligence.h"
#include "Skill.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class GotoOld : public Skill
		{
			Q_OBJECT

		public:
			GotoOld(QObject *parent, Robot* slave, qreal x=0.0, qreal y=0.0);

			void step();

		protected:
			qreal x, y;
	
		};
	}
}

#endif // GOTOOLD_H
