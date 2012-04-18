#include "DriveToBall.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include <QLineF>

#define M_PI	3.1415926535897932
#define CART	82.6//110.//170.//82.6

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveToBall::DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed, bool deterministic)
	: DriveToObject(parent, slave, slave->stage()->ball(), refLookPoint, speed, deterministic)
{
	threshold = CART;
}

DriveToBall::~DriveToBall(void)
{
}

//Robo se posiciona primeiro numa posicao de tal forma que nao colida com a bola depois vai para a posicao de pegar a bola
void DriveToBall::step()
{
	Ball* ball = stage()->ball();
	Robot* robot = this->robot();
	qreal t = -4*CART;

	//Construindo area de cone para pegar a bola
	const Object* lkP = getRefLookPoint();
	QLineF target = QLineF(ball->x(), ball->y(), lkP->x(), lkP->y());
	qreal angle = 80;
	target.setLength(t);
	target.setAngle(target.angle() + angle);
	QLineF target2 = target;
	target.setAngle(target.angle() - 2*angle);
	QLineF target3 = QLineF(ball->x(), ball->y(), robot->x(), robot->y());

	qreal ang2 = target2.angle();
	qreal ang3 = target3.angle();
	qreal ang = target.angle();
	if(!( (ang2 > ang && ( ang3 < ang2 && ang3 > ang ) ) || (ang2 < ang && ( ang3 < ang2 || ang3 > ang ) ) )){ //nao esta dentro do cone
		qreal goAng = 15;
		target.setAngle(target.angle() + goAng);
		target2.setAngle(target2.angle() - goAng);
		Object aux2 = Object(target.p2().x(),target.p2().y());
		Object aux1 = Object(target2.p2().x(),target2.p2().y());

		Object newLKP;

		if((robot->distance(&aux1)).module() < (robot->distance(&aux2)).module())
			newLKP = Object(aux1);
		else
			newLKP = Object(aux2);

		QLineF target1 = QLineF(ball->x(), ball->y(), newLKP.x(), newLKP.y());
		target1.setLength(t);
		setRefLookPoint(&Object(target1.p2().x(),target1.p2().y()));

		qreal backup = threshold;
		threshold = -t;
		DriveToObject::step();
		threshold = backup;

		setRefLookPoint(lkP);
		//cout << "FORA" << endl;
	}
	else{ //esta dentro do cone
		DriveToObject::step();
		
		//cout << "DENTRO" << endl;
	}
}

bool DriveToBall::busy()
{
	return DriveToObject::busy();
}