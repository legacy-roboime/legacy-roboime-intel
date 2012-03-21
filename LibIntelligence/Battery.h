#ifndef BATTERY_H
#define BATTERY_H

#include "Component.h"

namespace LibIntelligence
{
	class Battery : public Component
	{
		Q_OBJECT

	public:
		Battery(const Battery&);
		Battery(QObject *parent=0);

		//TODO

	private:
	
	};
}

#endif // BATTERY_H
