#include "UpdateStageGeometry.h"
#include "Updater.h"
#include "Goal.h"
#include <iostream>
#include <sstream>
#include "messages_robocup_ssl_geometry.pb.h"

namespace LibIntelligence
{
	struct UpdateStageGeometryImpl
	{
		UpdateStageGeometryImpl(qreal goal_width__, qreal goal_depth__, qreal goal_wallWidth__, qreal lineWidth__, qreal fieldLength__, 
			qreal fieldWidth__, qreal boundaryWidth__, qreal refereeWidth__, qreal centerCircleRadius__,
			qreal defenseRadius__, qreal defenseStretch__, qreal freeKickDistance__, qreal penaltySpotDistance__, 
			qreal penaltyLineDistance__) 
			: goal_width_(goal_width__), goal_depth_(goal_depth__), goal_wallWidth_(goal_wallWidth__), lineWidth_(lineWidth__), 
			fieldLength_(fieldLength__), fieldWidth_(fieldWidth__), boundaryWidth_(boundaryWidth__), refereeWidth_(refereeWidth__), 
			centerCircleRadius_(centerCircleRadius__), defenseRadius_(defenseRadius__), defenseStretch_(defenseStretch__), 
			freeKickDistance_(freeKickDistance__), penaltySpotDistance_(penaltySpotDistance__), penaltyLineDistance_(penaltyLineDistance__) {}

		qreal goal_width_;
		qreal goal_depth_;
		qreal goal_wallWidth_;
		qreal lineWidth_;
		qreal fieldLength_;
		qreal fieldWidth_;
		qreal boundaryWidth_;
		qreal refereeWidth_;
		qreal centerCircleRadius_;
		qreal defenseRadius_;
		qreal defenseStretch_;
		qreal freeKickDistance_;
		qreal penaltySpotDistance_;
		qreal penaltyLineDistance_;
	};
}

using namespace LibIntelligence;

UpdateStageGeometry::UpdateStageGeometry(const SSL_GeometryData& p, double t_sent, double t_capture, int camera_id)
	: Update(t_sent,t_capture,camera_id) {
		const SSL_GeometryFieldSize f = p.field();
		pimpl = new UpdateStageGeometryImpl(f.goal_width(), f.goal_depth(), f.goal_wall_width(), f.line_width(),
			f.field_length(), f.field_width(), f.boundary_width(), f.referee_width(), f.center_circle_radius(), 
			f.defense_radius(), f.defense_stretch(), f.free_kick_from_defense_dist(), f.penalty_line_from_spot_dist(), 
			f.penalty_spot_from_field_line_dist());
}

UpdateStageGeometry::UpdateStageGeometry(qreal goal_width_, qreal goal_depth_, qreal goal_wallWidth_, qreal lineWidth_, qreal fieldLength_, 
			qreal fieldWidth_, qreal boundaryWidth_, qreal refereeWidth_, qreal centerCircleRadius_, 
			qreal defenseRadius_, qreal defenseStretch_, qreal freeKickDistance_, qreal penaltySpotDistance_, 
			qreal penaltyLineDistance_, double t_sent, double t_capture, int camera_id)
			: Update(t_sent,t_capture,camera_id), pimpl(new UpdateStageGeometryImpl(goal_width_, goal_depth_, goal_wallWidth_, lineWidth_, fieldLength_, 
			fieldWidth_, boundaryWidth_, refereeWidth_, centerCircleRadius_, defenseRadius_, defenseStretch_, freeKickDistance_, penaltySpotDistance_, 
			penaltyLineDistance_)) {}

UpdateStageGeometry::~UpdateStageGeometry()
{
	delete pimpl;
}

void UpdateStageGeometry::apply(Updater* u)
{
	//cout << to_string() << endl;//test
	for(quint32 k = u->stagesSize(); k > 0; k--) {
		Stage* stage = u->stage(k-1);
		Goal* blueGoal = stage->blueGoal();
		Goal* yellowGoal = stage->yellowGoal();
		if(stage->isLeftSideBlueGoal()){
			blueGoal->setX(-fieldLength()/2.);
			blueGoal->setY(0);
			blueGoal->setDepth(goal_depth());
			blueGoal->setWallWidth(goal_wallWidth());
			blueGoal->setWidth(goal_width());
			blueGoal->setPenaltyLine(-fieldLength()/2. + stage->penaltySpotDistance() + stage->penaltyLineDistance());
			blueGoal->setPenaltyMark(-fieldLength()/2. + stage->penaltySpotDistance(), 0);

			yellowGoal->setX(fieldLength()/2.);
			yellowGoal->setY(0);
			yellowGoal->setDepth(goal_depth());
			yellowGoal->setWallWidth(goal_wallWidth());
			yellowGoal->setWidth(goal_width());
			yellowGoal->setPenaltyLine(fieldLength()/2. - stage->penaltySpotDistance() - stage->penaltyLineDistance());
			yellowGoal->setPenaltyMark(fieldLength()/2. - stage->penaltySpotDistance(), 0);
		}
		else{
			blueGoal->setX(fieldLength()/2.);
			blueGoal->setY(0);
			blueGoal->setDepth(goal_depth());
			blueGoal->setWallWidth(goal_wallWidth());
			blueGoal->setWidth(goal_width());
			blueGoal->setPenaltyLine(fieldLength()/2. - stage->penaltySpotDistance() - stage->penaltyLineDistance());
			blueGoal->setPenaltyMark(fieldLength()/2. - stage->penaltySpotDistance(), 0);

			yellowGoal->setX(-fieldLength()/2.);
			yellowGoal->setY(0);
			yellowGoal->setDepth(goal_depth());
			yellowGoal->setWallWidth(goal_wallWidth());
			yellowGoal->setWidth(goal_width());
			yellowGoal->setPenaltyLine(-fieldLength()/2. + stage->penaltySpotDistance() + stage->penaltyLineDistance());
			yellowGoal->setPenaltyMark(-fieldLength()/2. + stage->penaltySpotDistance(), 0);
		}

		stage->setBlueGoal(blueGoal);
		stage->setYellowGoal(yellowGoal);

		stage->setLineWidth(lineWidth());
		stage->setFieldLength(fieldLength());
		stage->setFieldWidth(fieldWidth());
		stage->setBoundaryWidth(boundaryWidth());
		stage->setRefereeWidth(refereeWidth());
		stage->setCenterCircleRadius(centerCircleRadius());
		stage->setDefenseRadius(defenseRadius());
		stage->setDefenseStretch(defenseStretch());
		stage->setFreeKickDistance(freeKickDistance());
		stage->setPenaltySpotDistance(penaltySpotDistance());
		stage->setPenaltyLineDistance(penaltyLineDistance());
	}
}

qreal UpdateStageGeometry::goal_width() const{
	return pimpl->goal_width_;
}

qreal UpdateStageGeometry::goal_depth() const{
	return pimpl->goal_depth_;
}

qreal UpdateStageGeometry::goal_wallWidth() const{
	return pimpl->goal_wallWidth_;
}

qreal UpdateStageGeometry::lineWidth() const
{
	return pimpl->lineWidth_;
}

qreal UpdateStageGeometry::fieldLength() const
{
	return pimpl->fieldLength_;
}

qreal UpdateStageGeometry::fieldWidth() const
{
	return pimpl->fieldWidth_;
}

qreal UpdateStageGeometry::boundaryWidth() const
{
	return pimpl->boundaryWidth_;
}

qreal UpdateStageGeometry::refereeWidth() const
{
	return pimpl->refereeWidth_;
}

qreal UpdateStageGeometry::centerCircleRadius() const
{
	return pimpl->centerCircleRadius_;
}

qreal UpdateStageGeometry::defenseRadius() const
{
	return pimpl->defenseRadius_;
}

qreal UpdateStageGeometry::defenseStretch() const
{
	return pimpl->defenseStretch_;
}

qreal UpdateStageGeometry::freeKickDistance() const
{
	return pimpl->freeKickDistance_;
}

qreal UpdateStageGeometry::penaltySpotDistance() const
{
	return pimpl->penaltySpotDistance_;
}

qreal UpdateStageGeometry::penaltyLineDistance() const
{
	return pimpl->penaltyLineDistance_;
}

string UpdateStageGeometry::to_string() const
{
	stringstream out;
	out << "UpdateStageGeometry: " ;
	return out.str();
}
