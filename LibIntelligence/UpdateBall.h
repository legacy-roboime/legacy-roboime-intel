#pragma once
#ifndef UPDATEBALL_H
#define UPDATEBALL_H

#include "LibIntelligence.h"
#include "Update.h"
#include "Ball.h"

class SSL_DetectionBall;

namespace LibIntelligence
{
	class Updater;

	struct UpdateBallImpl;

	class UpdateBall : public Update
	{
	public:
		UpdateBall(const SSL_DetectionBall&, double t_sent=0.0, double t_capture=0.0, int camera_id=0);
		UpdateBall(qreal x, qreal y, double t_sent=0.0, double t_capture=0.0, int camera_id=0);
		~UpdateBall();

		void apply(Updater*);//go through the updaters list and apply the update

		void setX(qreal);
		qreal x() const;

		void setY(qreal);
		qreal y() const;

		void setSpeedX(qreal sx);
		qreal speedX() const;

		void setSpeedY(qreal sy);
		qreal speedY() const;

	private:
		UpdateBallImpl* pimpl;
	};
}

#endif // UPDATEBALL_H
