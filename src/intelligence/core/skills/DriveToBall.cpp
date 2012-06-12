#include "DriveToBall.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include <QLineF>

//#define CART	90.//110.//82.6//170.//82.6

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveToBall::DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed, bool deterministic, qreal maxAngVar, qreal maxErrorD, qreal maxErrorA)
	: DriveToObject(parent, slave, slave->stage()->ball(), slave->stage()->ball()->radius(), refLookPoint, maxErrorD, maxErrorA, speed, deterministic, maxAngVar),
	inCone(false)
{
	//threshold = CART;
}

DriveToBall::~DriveToBall(void)
{
}

//Robo se posiciona primeiro numa posicao de tal forma que nao colida com a bola depois vai para a posicao de pegar a bola
void DriveToBall::step()
{
	Ball* ball = stage()->ball();
	Robot* robot = this->robot();

	qreal distance = 4*threshold;
	qreal t = -distance;

	//Cone maior
	const Object* lkP = getRefLookPoint();
	QLineF target = QLineF(ball->x(), ball->y(), lkP->x(), lkP->y());
	qreal angle = 75;
	target.setLength(t);
	target.setAngle(target.angle() + angle);
	QLineF target2 = target;
	target.setAngle(target.angle() - 2*angle);
	QLineF target3 = QLineF(ball->x(), ball->y(), robot->x(), robot->y());

	qreal ang2 = target2.angle(); //angulo do cone maior obtido girando no sentido horario
	qreal ang3 = target3.angle(); //angulo da linha que liga bola pro robo
	qreal ang = target.angle(); //angulo do cone maior obtido girando no sentido anti-horario

	//Cone Menor
	qreal goAng = 20;
	target.setAngle(target.angle() + goAng);
	target2.setAngle(target2.angle() - goAng);
	qreal ang4 = target.angle(); //angulo do cone menor obtido girando no sentido anti-horario
	qreal ang5 = target2.angle(); //angulo do cone menor obtido girando no sentido horario

	//Verificando mudança de estado
	if(inCone){
		if(!( (ang2 > ang && ( ang3 < ang2 && ang3 > ang ) ) || 
			  (ang2 < ang && ( ang3 < ang2 || ang3 > ang ) ) )){ //n esta dentro do cone maior

			inCone = false;
		}
	}
	else{
		if( (ang5 > ang4 && ( ang3 < ang5 && ang3 > ang4 ) ) || 
			(ang5 < ang4 && ( ang3 < ang5 || ang3 > ang4 ) ) ){ //esta dentro do cone menor

			inCone = true;
		}
	}

	//Posições destino (dentro do cone menor)
	goAng = 15;
	target.setAngle(target.angle() + goAng);
	target2.setAngle(target2.angle() - goAng);

	if(inCone){ //esta dentro do cone
		QLineF ret1 = QLineF(lkP->x(), lkP->y(), ball->x(), ball->y());
		ret1.setLength(ret1.length() + distance);
		QLineF ret2 = ret1;
		ret2.setAngle(ret2.angle() + 90);
		ret2.translate(robot->x() - ret2.p1().x(), robot->y() - ret2.p1().y());
		QPointF intersect = QPointF(0, 0);
		QLineF::IntersectType interT = ret1.intersect(ret2, &intersect);
		if(robot->distance(&Object(intersect.x(), intersect.y())).module() > 50){
			Goto::setPoint(ret1.x2(), ret1.y2());
			ret1.setLength(-1);
			Goto::setOrientation(2*M_PI - ret1.angle() * M_PI / 180.); //TODO: arrumar esse angulo para o robo nao fazer a voltinha.
			Goto::step();
		}
		else
			DriveToObject::step();
		//cout << "DENTRO" << endl;
	}
	else{ //nao esta dentro do cone

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

		//robot->dribble(0);
	}
}

bool DriveToBall::busy()
{
	return DriveToObject::busy();
}