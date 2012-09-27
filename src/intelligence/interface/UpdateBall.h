#pragma once
#ifndef UPDATEBALL_H
#define UPDATEBALL_H

#include "LibIntelligence.h"
#include "Update.h"
#include "Ball.h"

class SSL_DetectionBall;

namespace LibIntelligence
{
	struct Bola{
		float x;
		float y;
	};

	class Updater;

	class UpdateBall : public Update, public Point
	{
	public:
		UpdateBall(const SSL_DetectionBall&, double t_sent=0.0, double t_capture=0.0, int camera_id=0);
		UpdateBall(qreal x, qreal y, double t_sent=0.0, double t_capture=0.0, int camera_id=0);
		~UpdateBall();

		void apply(Updater*);//go through the updaters list and apply the update

		void setSpeed(const Vector &);
		void setSpeed(const qreal &sx, const qreal &sy);
		Vector speed() const;

	private:
		struct UpdateBallImpl* pimpl;
	};
}

#endif // UPDATEBALL_H
