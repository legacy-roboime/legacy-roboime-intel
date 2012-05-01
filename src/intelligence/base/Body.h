#pragma once
#ifndef BODY_H
#define BODY_H

#include "LibIntelligence.h"
#include "Component.h"

namespace LibIntelligence
{
	class Body : public Component
	{
		Q_OBJECT

	public:
		Body(const Body&);
		Body(QObject* parent=0, qreal height=0.0, qreal radius=0.0);

		void setHeight(qreal);
		qreal height() const;

		void setRadius(qreal);
		qreal radius() const;

	private:
		qreal height_;
		qreal radius_;
	};
}

#endif // BODY_H
