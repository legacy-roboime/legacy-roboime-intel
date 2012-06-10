#include "Skills.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;
using namespace LibIntelligence::Skills::Loops;

Orbit::Orbit(QObject* p, Robot* r, qreal x, qreal y, qreal d, qreal s, qreal a)
	: Goto(p, r, 0, 0, 0, s),
	centerX(x),
	centerY(y),
	radius(d),
	speedLinear(s),
	speedAngular(a)
{
	ignoreBrake = true;
}

void Orbit::setAll(qreal x, qreal y, qreal d, qreal s, qreal a) {
	centerX = x; centerY = y; radius = d; speedLinear = s; speedAngular = a;
}

void Orbit::step() {
	qreal d, dx, dy, n, costheta, sintheta;
	dx = centerX - robot()->x();
	dy = centerY - robot()->y();
	n = dx*dx + dy*dy;
	if(n>=radius*radius) {
		d = sqrt(n - radius*radius);
		costheta = (dx*d + dy*radius)/n;
		sintheta = (dy*d - dx*radius)/n;
		Goto::setPoint(centerX + radius * sintheta, centerY - radius * costheta);
		Goto::step();
		Move::setAll(abs(speedLinear) * costheta, abs(speedLinear) * sintheta, 0.0);
		//Move::step();
	} else {
		Move::step();
	}
}

Circle::Circle(QObject* p, Robot* r, qreal x, qreal y, qreal d, qreal s) : Orbit(p, r, x, y, d, s, 0.0) {}
