#pragma once
#ifndef DRIBBLER_H
#define DRIBBLER_H

#include "LibIntelligence.h"
#include "Actuator.h"

namespace LibIntelligence
{
	class Dribbler : public Actuator
	{
		Q_OBJECT

	public:
		Dribbler(const Dribbler&);
		Dribbler(QObject* parent=0, qreal speed=0.0);

	private:
	};
}

#endif // DRIBBLER_H
