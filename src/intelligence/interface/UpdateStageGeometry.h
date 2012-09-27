#pragma once
#ifndef UPDATESTAGE_H
#define UPDATESTAGE_H

#include "LibIntelligence.h"
#include "Update.h"
#include "Stage.h"

class SSL_DetectionRobot;
class SSL_GeometryData;
class SSL_GeometryFieldSize;

using namespace std;

namespace LibIntelligence
{
	struct Campo{
		float length;
		float width;
	};

	class Updater;

	struct UpdateStageGeometryImpl;

	class UpdateStageGeometry : public Update
	{
	public:
		UpdateStageGeometry(const SSL_GeometryData&, double t_sent=0.0, double t_capture=0.0, int camera_id=0);
		UpdateStageGeometry(qreal goal_width_=0.0, qreal goal_depth_=0.0, qreal goal_wallWidth_=0.0, qreal lineWidth_=0.0, qreal fieldLength_=0.0, 
			qreal fieldWidth_=0.0, qreal boundaryWidth_=0.0, qreal refereeWidth_=0.0, qreal centerCircleRadius_=0.0, 
			qreal defenseRadius_=0.0, qreal defenseStretch_=0.0, qreal freeKickDistance_=0.0, qreal penaltySpotDistance_=0.0, 
			qreal penaltyLineDistance_=0.0, double t_sent=0.0, double t_capture=0.0, int camera_id=0);
		~UpdateStageGeometry();

		void apply(Updater*);//go through the updaters list and apply the update
		string to_string() const;

		qreal goal_width() const;
		qreal goal_depth() const; 
		qreal goal_wallWidth() const;
		qreal lineWidth() const;
		qreal fieldLength() const;
		qreal fieldWidth() const;
		qreal boundaryWidth() const;
		qreal refereeWidth() const;
		qreal centerCircleRadius() const;
		qreal defenseRadius() const;
		qreal defenseStretch() const;
		qreal freeKickDistance() const;
		qreal penaltySpotDistance() const;
		qreal penaltyLineDistance() const;
	private:
		UpdateStageGeometryImpl* pimpl;
	};
}

#endif // UPDATESTAGE_H