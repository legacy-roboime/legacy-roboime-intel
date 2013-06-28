#include "Robots.h"
#include <QVector>

using namespace LibIntelligence;
using namespace LibIntelligence::Robots;

//RoboIME2013 robot assembly line:
const QVector<Wheel> RoboIME2013_wheels()
{
	QVector<Wheel> wheels;
	wheels
		//TODO: correct the distance of the wheel to the center of the robot.
		<< Wheel(0, -1.0471975511965976, 28.9, 80.6)
		<< Wheel(0, -2.3561944901923450, 28.9, 80.6)
		<< Wheel(0,  2.3561944901923450, 28.9, 80.6)
		<< Wheel(0, -1.0471975511965976, 28.9, 80.6);
	return wheels;
}

const Robot& LibIntelligence::Robots::RoboIME2013(QObject* p, int id, int pid, TeamColor c) {
	static Robot RoboIME2013_(
		Dribbler(),
		Kicker(),
		Body(0, 149.5, 90.0, 70.),
		Battery(),
		RoboIME2013_wheels());

	RoboIME2013_.setParent(p);
	RoboIME2013_.setId(id);
	RoboIME2013_.setPatternId(pid);
	RoboIME2013_.setColor(c);

	return RoboIME2013_;
}

//RoboIME2012 robot assembly line:
const QVector<Wheel> RoboIME2012_wheels()
{
	QVector<Wheel> wheels;
	wheels  << Wheel(0, -0.99483767363676785884650373803851, 28.9, 80.6)
	        << Wheel(0,  0.99483767363676785884650373803851, 28.9, 80.6)
			<< Wheel(0,  2.3561944901923449288469825374596, 28.9, 80.6)
			<< Wheel(0, -2.3561944901923449288469825374596, 28.9, 80.6);
	return wheels;
}

const Robot& LibIntelligence::Robots::RoboIME2012(QObject* p, int id, int pid, TeamColor c) {
	static Robot RoboIME2012_(
		Dribbler(),
		Kicker(),
		Body(0, 137.8, 87.5, 70.),
		Battery(),
		RoboIME2012_wheels());

	RoboIME2012_.setParent(p);
	RoboIME2012_.setId(id);
	RoboIME2012_.setPatternId(pid);
	RoboIME2012_.setColor(c);

	return RoboIME2012_;
}

//RoboIME2011 robot assembly line:
const QVector<Wheel> RoboIME2011_wheels()
{
	QVector<Wheel> wheels;
	wheels  << Wheel(0, -0.99483767363676785884650373803851, 28.9, 80.6)
	        << Wheel(0,  0.99483767363676785884650373803851, 28.9, 80.6)
			<< Wheel(0,  2.3561944901923449288469825374596, 28.9, 80.6)
			<< Wheel(0, -2.3561944901923449288469825374596, 28.9, 80.6);
	return wheels;
}

const Robot& LibIntelligence::Robots::RoboIME2011(QObject* p, int id, int pid, TeamColor c) {
	static Robot RoboIME2011_(
		Dribbler(),
		Kicker(),
		Body(0, 137.8, 87.5, 70.),
		Battery(),
		RoboIME2011_wheels());

	RoboIME2011_.setParent(p);
	RoboIME2011_.setId(id);
	RoboIME2011_.setPatternId(pid);
	RoboIME2011_.setColor(c);

	return RoboIME2011_;
}

//RoboIME2010 robot assembly line:
const QVector<Wheel> RoboIME2010_wheels()
{
	QVector<Wheel> wheels;
	wheels  << Wheel(0, -1,5707963, 30, 75)
	        << Wheel(0,  1,5707963, 30, 75);
	return wheels;
}

const Robot& LibIntelligence::Robots::RoboIME2010(QObject* p, int id, int pid, TeamColor c) {
	static Robot RoboIME2010_(
		Dribbler(),
		Kicker(),
		Body(0, 100, 80, 70.),
		Battery(),
		RoboIME2011_wheels());

	RoboIME2010_.setParent(p);
	RoboIME2010_.setId(id);
	RoboIME2010_.setPatternId(pid);
	RoboIME2010_.setColor(c);

	return RoboIME2010_;
}
//TODO: make more assembly lines
