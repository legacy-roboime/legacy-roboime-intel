#include "KickTo.h"
#include "GetBall.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "geomutils.h"

#define M_PI	3.1415926535897932

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

KickTo::KickTo(QObject* p, Robot* r, qreal a)
	: GetBall(p, r, 1600.0),
	angle(a) {}

void KickTo::setAngle(qreal _angle)
{
	angle = _angle;
}

qreal KickTo::getAngle()
{
	return angle;
}

void KickTo::step()
{
	static Robot* robot = this->robot();

	if(hasBall()) {
		robot->kick();
		GetBall::step();
	}
	else{
		robot->kick(0);
		GetBall::step();
	}

	/*static qreal dx, dy;

	qreal rOrientation = robot()->orientation();

	qreal xRobot = robot()->x();
	qreal yRobot = robot()->y();
	qreal xBall = stage()->ball()->x();
	qreal yBall = stage()->ball()->y();

	dx = xBall - xRobot;
	dy = yBall - yRobot;
	Goto::setOrientation(angle);

	//TODO: find geometry lib (QT) 
	Line lineTo = Line(xRobot, yRobot, xRobot + 1., yRobot);
	Line lineToBall = Line(xRobot, yRobot, xBall, yBall);

	//Verifica alinhamento da bola com a frente do robo (sera substituido pelo sensor da bola)
	lineTo.setAngle(rOrientation*180./M_PI);
	qreal diffAngle = lineTo.angleTo(lineToBall) * M_PI / 180.;

	qreal testa = abs(rOrientation-angle);
	if(sqrt(dx * dx + dy * dy) < robot()->body().radius() + 2 * stage()->ball()->radius() && testa < M_PI/15. && abs(diffAngle) < M_PI / 15.) {
		robot()->kick();
		//printf("CHUTEI %f\n", testa);
	}
	else{
		robot()->kick(0);
	}

	static long wait =0;
	if(sqrt(dx * dx + dy * dy) > robot()->body().radius() + 2 * stage()->ball()->radius() || abs(diffAngle) > M_PI / 90. //|| wait < 200//){
		GetBall::step();
		//printf("GETBALL %d\n",teste);
		wait++;
	}
	else{
		Goto::step();
		//printf("ALINHEI %d\n",wait);
		wait=0;
	}*/
}
