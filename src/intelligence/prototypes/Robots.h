#pragma once
#ifndef ROBOTS_H
#define ROBOTS_H

#include "Robot.h"

namespace LibIntelligence
{
	namespace Robots
	{
		const Robot& RoboIME2012(QObject* parent=0, int id=0, int colorCode=0, TeamColor color=BLUE);
		const Robot& RoboIME2011(QObject* parent=0, int id=0, int colorCode=0, TeamColor color=BLUE);
		const Robot& RoboIME2010(QObject* parent=0, int id=0, int colorCode=0, TeamColor color=BLUE);
		//TODO: make more prototypes
	}
}

#endif // ROBOTS_H
