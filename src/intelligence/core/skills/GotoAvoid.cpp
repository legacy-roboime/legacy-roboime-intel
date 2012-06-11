#include "GotoAvoid.h"
#include "mathutils.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

GotoAvoid::GotoAvoid(QObject *parent, Robot *slave, qreal targetX, qreal targetY, qreal avoidX, qreal avoidY, qreal speed)
	: Goto(parent, slave, targetX, targetY, 0.0, speed),
	avoidX(avoidX),
	avoidY(avoidY)
{}

void GotoAvoid::setTarget(qreal x, qreal y)
{
	Goto::setPoint(x, y);
}

void GotoAvoid::setAvoid(qreal x, qreal y)
{
	avoidX = x;
	avoidY = y;
}

void GotoAvoid::setSpeed(qreal s)
{
	Goto::setSpeed(s);
}

void GotoAvoid::step()
{
	//T: target, A: avoid, R: robot
	qreal dAR2 = SQ(avoidX - robot()->x()) + SQ(avoidY - robot()->y());
	qreal dRT2 = SQ(targetX - robot()->x()) + SQ(targetY - robot()->y());
	qreal dTA2 = SQ(targetX - avoidX) + SQ(targetY - avoidY);
	qreal dTAdRT = sqrt(dTA2 * dRT2);
	qreal cosATR = (dAR2 - dTA2 - dRT2) / 2 / dTAdRT;

	if(cosATR <= 0) {
		// Ir direto
		Goto::step();
	} else {
		// Ir para a tangente primeiro
		//TODO
	}
}