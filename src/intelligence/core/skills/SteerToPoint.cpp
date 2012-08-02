#include "SteerToPoint.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "geomutils.h"

using namespace LibIntelligence;
using namespace Skills;

SteerToPoint::SteerToPoint(QObject* p, Robot* r, Point *t, Vector s)
	: Steer(p, r, s.x(), s.y()),
	point_(t) {}

void SteerToPoint::step()
{
	Line line(*robot(), *point_);
	Steer::setOrientation(line.dx(), line.dy());
	Steer::step();
}
