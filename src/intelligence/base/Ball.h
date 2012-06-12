#pragma once
#ifndef BALL_H
#define BALL_H

#include "LibIntelligence.h"
#include "Object.h"
#include <QObject>

namespace LibIntelligence
{
	class Ball : public Object
	{
	public:
		Ball(qreal radius=0.0);
		Ball(const Ball& ball);

		void setRadius(qreal);
		qreal radius() const;

	private:
		qreal radius_;
	};
}

#endif // BALL_H
