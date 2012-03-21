#include "SteerToBall.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"

using namespace LibIntelligence;
using namespace Skills;

SteerToBall::SteerToBall(QObject* p, Robot* s, qreal sx, qreal sy)
	: Steer(p, s, sx, sy, 0.0)
{
	Steer::setOrientation(stage()->ball()->x() - robot()->x(), stage()->ball()->y() - robot()->y());
}

void SteerToBall::step()
{
	Steer::setOrientation(stage()->ball()->x() - robot()->x(), stage()->ball()->y() - robot()->y());
	Steer::step();
}
