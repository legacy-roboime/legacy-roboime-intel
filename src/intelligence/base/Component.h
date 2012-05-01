#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include "LibIntelligence.h"
#include <QObject>
#include <QString>

namespace LibIntelligence {

	class Component : public QObject
	{
		Q_OBJECT

	public:
		Component(const Component&);
		Component(QObject* parent=0, bool active=true, bool working=true);

		Component& operator=(const Component& other);

		bool isWorking() const;
		bool isNotWorking() const;
		void setWorking();
		void setNotWorking();

		bool isActive() const;
		bool isNotActive() const;
		void setActive();
		void setNotActive();

	signals:
		void stoppedWorking();
		void startedWorking();//emitted only if was broken before
		void deactivated();
		void reactivated();

	protected:
		bool active, working;
	};
}

#endif // COMPONENT_H
