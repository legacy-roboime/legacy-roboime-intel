#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Component.h"

namespace LibIntelligence
{
	class Actuator : public Component
	{
		Q_OBJECT

	public:
		Actuator(const Actuator&);
		Actuator(QObject* parent=0, qreal speed=0.0);

		void setSpeed(qreal);
		qreal speed() const;

	private:
		qreal speed_;
	};
}


#endif // ACTUATOR_H
