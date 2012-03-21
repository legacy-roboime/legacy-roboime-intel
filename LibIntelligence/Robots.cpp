#include "Robots.h"
#include <QVector>

using namespace LibIntelligence;
using namespace LibIntelligence::Robots;

//RobotIME2011 robot assembly line:
const QVector<Wheel> RobotIME2011_wheels()
{
	QVector<Wheel> wheels;
	wheels  << Wheel(0, -0.99483767363676785884650373803851, 28.9, 80.6)
	        << Wheel(0,  0.99483767363676785884650373803851, 28.9, 80.6)
			<< Wheel(0,  2.3561944901923449288469825374596, 28.9, 80.6)
			<< Wheel(0, -2.3561944901923449288469825374596, 28.9, 80.6);
	return wheels;
}

const Robot& LibIntelligence::Robots::RobotIME2011(QObject* p, int id, int pid, TeamColor c) {
	static Robot RobotIME2011_(
		Dribbler(),
		Kicker(),
		Body(0, 137.8, 87.5),
		Battery(),
		RobotIME2011_wheels());

	RobotIME2011_.setParent(p);
	RobotIME2011_.setId(id);
	RobotIME2011_.setPatternId(pid);
	RobotIME2011_.setColor(c);

	return RobotIME2011_;
}
//RobotIME2011 robot assembly line:
const QVector<Wheel> RobotIME2010_wheels()
{
	QVector<Wheel> wheels;
	wheels  << Wheel(0, -1,5707963, 30, 75)
	        << Wheel(0,  1,5707963, 30, 75);
	return wheels;
}

const Robot& LibIntelligence::Robots::RobotIME2010(QObject* p, int id, int pid, TeamColor c) {
	static Robot RobotIME2010_(
		Dribbler(),
		Kicker(),
		Body(0, 100, 80),
		Battery(),
		RobotIME2011_wheels());

	RobotIME2010_.setParent(p);
	RobotIME2010_.setId(id);
	RobotIME2010_.setPatternId(pid);
	RobotIME2010_.setColor(c);

	return RobotIME2010_;
}
//TODO: make more assembly lines