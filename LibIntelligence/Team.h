#pragma once
#ifndef TEAM_H
#define TEAM_H

#include "LibIntelligence.h"
#include <QObject>
#include <QList>
#include "Robot.h"

namespace LibIntelligence
{
	class Team : public QObject, public QList<Robot*>
	{
		Q_OBJECT

	public:
		Team(QObject* parent=0, TeamColor=BLUE);
		Team(Stage* stage, const Team& team);

		void setYellowCard(quint8);
		quint8 yellowCards() const;

		void setRedCard(quint8);
		quint8 redCards() const;

		void setColor(TeamColor);
		TeamColor color() const;

		void setGoals(quint8);
		quint8 goals() const;

		void addYellowCard();
		void addRedCard();

		Team* enemyTeam();
		Stage* stage();
		Goal* goal();
		Goal* enemyGoal();

		Team& operator=(const Team& team);

	private:
		TeamColor color_;
		quint8 goals_;
		quint8 yellowCards_, redCards_;
		Stage* stage_;
	};
}

#endif // TEAM_H
