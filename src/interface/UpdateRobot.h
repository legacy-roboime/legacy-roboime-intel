#pragma once
#ifndef UPDATEROBOT_H
#define UPDATEROBOT_H

#include "LibIntelligence.h"
#include "Update.h"
#include "Robot.h"

class SSL_DetectionRobot;

namespace LibIntelligence
{
	class Updater;

	struct UpdateRobotImpl;

	class UpdateRobot : public Update, public Point
	{
	public:
		UpdateRobot(TeamColor, const SSL_DetectionRobot&, double t_sent=0.0, double t_capture=0.0, int camera_id=0, bool active=false);
		UpdateRobot(TeamColor, quint8 patternId, qreal x, qreal y, qreal theta, double t_sent=0.0, double t_capture=0.0, int camera_id=0, bool active=true);
		~UpdateRobot();

		void apply(Updater*);//go through the updaters list and apply the update
		string to_string() const;

		TeamColor color() const;
		quint8 patternId() const;

		void setSpeed(const Vector &);
		void setSpeed(const qreal &sx, const qreal &sy);
		Vector speed() const;

		void setTheta(qreal);
		qreal theta() const;

		void setSpeedTheta();
		qreal speedTheta() const;

		void setActive(bool active);
		bool active() const;

	private:
		UpdateRobotImpl* pimpl;
		qreal last_time_capture;
	};
}

#endif // UPDATEROBOT_H
