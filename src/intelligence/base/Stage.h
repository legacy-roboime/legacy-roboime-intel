#pragma once
#ifndef STAGE_H
#define STAGE_H

#include "LibIntelligence.h"
#include "Robot.h"
#include "Team.h"
#include <QObject>
#include <map>

namespace LibIntelligence
{
	class Ball;
	class Goal;
	class Team;

	class Stage : public QObject
	{
		friend class Robot;

		Q_OBJECT

	public:
		Stage();
		Stage(const Stage& stage);
		~Stage();
		//TODO: too many fields, think about it.

		/*const*/ Ball* ball() const;
		/*const*/ Goal* blueGoal() const;
		/*const*/ Goal* yellowGoal() const;
		/*const*/ Team* blueTeam() const;
		/*const*/ Team* yellowTeam() const;

		Ball* ball();
		Goal* blueGoal();
		Goal* yellowGoal();
		Team* blueTeam();
		Team* yellowTeam();

		void setBlueTeam(Team*);
		void setYellowTeam(Team*);

		void setBall(Ball*);
		void setBlueGoal(Goal*);
		void setYellowGoal(Goal*);


		void setLineWidth(qreal);
		qreal lineWidth() const;

		void setFieldLength(qreal);
		qreal fieldLength() const;

		void setFieldWidth(qreal);
		qreal fieldWidth() const;

		void setBoundaryWidth(qreal);
		qreal boundaryWidth() const;

		void setRefereeWidth(qreal);
		qreal refereeWidth() const;

		void setCenterCircleRadius(qreal);
		qreal centerCircleRadius() const;

		void setDefenseRadius(qreal);
		qreal defenseRadius() const;

		void setDefenseStretch(qreal);
		qreal defenseStretch() const;

		void setFreeKickDistance(qreal);
		qreal freeKickDistance() const;

		void setPenaltySpotDistance(qreal);
		qreal penaltySpotDistance() const;

		void setPenaltyLineDistance(qreal);
		qreal penaltyLineDistance() const;

		void setCmdReferee(char);
		char getCmdReferee() const;

		void setTimeLeft(double);
		double getTimeLeft() const;

		Robot* getClosestPlayerToBall(const Team*) const;
		Robot* getClosestPlayerToBallThatCanKick(const Team*) const;
		map<qreal, Robot*> getClosestPlayersToBall(const Team* team) const;
		map<qreal, Robot*> getClosestPlayersToBallThatCanKick(const Team* team) const;
		Robot* getClosestOrderPlayerToBall(const Team* team, int order) const;
		Team* getTeamFromColor(const TeamColor& color);
		Team* getTeamFromOtherColor(const TeamColor& color);
		Goal* getGoalFromOtherColor(const TeamColor& color);
		Goal* getGoalFromColor(const TeamColor& color);
		bool isGoal(const TeamColor& color);
		bool inField(const Object& obj);

		Stage& operator=(const Stage& stage);
		bool operator==(const Stage& stage);
		bool operator==(const Stage* stage);

		bool isLeftSideBlueGoal();
	private:
		qreal lineWidth_;
		qreal fieldLength_;
		qreal fieldWidth_;
		qreal boundaryWidth_;
		qreal refereeWidth_;
		qreal centerCircleRadius_;
		qreal defenseRadius_;
		qreal defenseStretch_;
		qreal freeKickDistance_;
		qreal penaltySpotDistance_;//from field
		qreal penaltyLineDistance_;//from spot

		char cmdReferee_;
		double _time_left;
		//fields:
		Ball* ball_;
		Goal* blueGoal_;
		Goal* yellowGoal_;
		Team* blueTeam_;
		Team* yellowTeam_;
		bool isLeftSideBlueGoal_;
	};
}

#endif // STAGE_H
