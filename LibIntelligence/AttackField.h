#pragma once
#ifndef ATTACKFIELD_H
#define ATTACKFIELD_H

#include "LibIntelligence.h"
#include "ForceFieldMotion.h"

namespace LibIntelligence
{
	namespace Skills
	{
		class AttackField : public ForceFieldMotion
		{
			Q_OBJECT

		public:
			AttackField(QObject* parent, Robot* slave, qreal x=0.0, qreal y=0.0, qreal orientation=0.0, qreal speed=0.0);

			void step(bool emitting = false);
		};
	}
}

#endif // ATTACKFIELD_H
