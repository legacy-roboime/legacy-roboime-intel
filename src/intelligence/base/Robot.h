#pragma once
#ifndef ROBOT_H
#define ROBOT_H

#include "LibIntelligence.h"
#include "Component.h"
#include "Object.h"
#include "Battery.h"
#include "Body.h"
#include "Dribbler.h"
#include "Kicker.h"
#include "Wheel.h"
#include "Command.h"
#include <QVector>

using namespace std;

namespace LibIntelligence
{
	enum TeamColor {YELLOW, BLUE};

	class Stage;
	class Team;
	class Goal;

	class Robot : public Component, public Object
	{
		Q_OBJECT

	public:
		Robot(const Robot&);
		//regular use constructor:
		Robot(QObject* parent=0, int id=0, int colorCode=0, TeamColor color=BLUE);
		//factory constructor for prototyping:
		Robot(const Dribbler&, const Kicker&, const Body&, const Battery&, const QVector<Wheel>&);
		//TODO: make prototype class and only allow
		//		factory and prototye constructors
		~Robot();

		const Dribbler& dribbler() const;
		const Kicker& kicker() const;
		const Body& body() const;
		const Battery& battery() const;
		const QVector<Wheel>& wheels() const;

		Dribbler& dribbler();
		Kicker& kicker();
		Body& body();
		Battery& battery();
		QVector<Wheel>& wheels();

		Stage* stage() const;
		void setStage(Stage* stage);
		Team* team() const;
		void setTeam(Team* team);
		Team* enemyTeam() const;
		Goal* goal() const;
		Goal* enemyGoal() const;

		quint32 wheelsSize() const;

		void setCommand(const Command&);
		const Command& command() const;
		Command& command();
		void setCommandWheels(const QVector<qreal>& speed);
		void newCommand();
		void kick(qreal=1.0);
		void dribble(qreal=1.0);

		void setColor(TeamColor);
		TeamColor color() const;

		void setId(quint8);
		quint8 id() const;

		void setPatternId(quint8);
		quint8 patternId() const;

		void setAllWorking();
		void setAllNotWorking();

		bool canKick() const;
		bool canDribble() const;
		bool isWorking() const;
		bool isNotWorking() const;
		
		string toString() const;
		void setParent(QObject*);

		void setLastToucher(bool value)  { isLastToucher_ =value;}
		bool isLastToucher() const { return isLastToucher_;}
		bool checkIfTouched();
		void setTouchedBall(bool value) { touchedBall_=value;}
		bool touchedBall() const { return touchedBall_;}

	private:
		unsigned char ifTouchedState;
		bool isLastToucher_;
		bool touchedBall_;

		Dribbler dribbler_;
		Kicker kicker_;
		Body body_;
		Battery battery_;
		QVector<Wheel> wheels_;

		Stage* stage_;
		Team* team_;
		//Team*const* enemyTeam_;
		//Goal*const* goal_;
		//Goal*const* enemyGoal_;

		quint8 id_;
		quint8 patternId_;
		TeamColor color_;
		Command command_;

		string _summary() const;
	};
}

#endif // ROBOT_H