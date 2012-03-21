#pragma once
#ifndef BALL_H
#define BALL_H

#include "LibIntelligence.h"
#include "Object.h"
#include <QObject>

namespace LibIntelligence
{
	class Ball : public QObject, public Object
	{
		Q_OBJECT

	public:
		Ball(QObject* parent=0, qreal radius=0.0);
		Ball(const Ball& ball);

		void setRadius(qreal);
		qreal radius() const;

		Ball& operator=(const Ball& ball);
	private:
		qreal radius_;
	};
}

#endif // BALL_H
