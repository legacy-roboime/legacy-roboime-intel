#pragma once
#ifndef KICKER_H
#define KICKER_H

#include "LibIntelligence.h"
#include "Actuator.h"

namespace LibIntelligence
{
	class Kicker : public Actuator
	{
		Q_OBJECT

	public:
		Kicker(const Kicker&);
		Kicker(QObject* parent=0, qreal speed=0.0);

	private:
	};
}

#endif // KICKER_H
