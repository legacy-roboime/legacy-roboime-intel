#include "Wait.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

Wait::Wait(QObject* p, Robot* r) : Skill(p, r) {}

void Wait::step()
{
	QVector<qreal> speed(robot()->wheelsSize(), 0.0);
	robot()->kick(0.0);
	robot()->dribble(0.0);
	robot()->setCommandWheels(speed);
}

bool Wait::busy()
{
	return false;
}