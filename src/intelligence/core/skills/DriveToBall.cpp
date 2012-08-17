#include "DriveToBall.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include "mathutils.h"
#include "geomutils.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveToBall::DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed, bool deterministic, qreal maxAngVar, qreal maxErrorD, qreal maxErrorA)
	: DriveToObject(parent, slave, slave->stage()->ball(), slave->stage()->ball()->radius(), refLookPoint, maxErrorD, maxErrorA, speed, deterministic, maxAngVar),
	gotoa(new GotoAvoid(this, slave, new Point(0,0), slave->stage()->ball(), 2*threshold, speed)),
	inCone(false)
{}

DriveToBall::~DriveToBall(void)
{
	delete gotoa;
}

//Robo se posiciona primeiro numa posicao de tal forma que nao colida com a bola depois vai para a posicao de pegar a bola
//Robo primeiramente vai pro ponto de tangencia da reta que passa pela sua posição e é tangente a circunferencia de centro na bola e raio radiusAvoid
//Em seguida robo atinge a posição oposta ao lookPoint na circunferencia, atingindo waypoints em pontos intermediarios da circunferencia
//Em seguida robo se aproxima da bola
void DriveToBall::step()
{
	Ball* ball = stage()->ball();
	Robot* robot = this->robot();
	
	const Object* lkP = getRefLookPoint();
	Line ball_target = Line(ball->x(), ball->y(), lkP->x(), lkP->y());
	ball_target.setLength(-gotoa->getRadiusAvoid());
	qreal btAngle = ball_target.angle();

	Line ball_robot = Line(ball->x(), ball->y(), robot->x(), robot->y());

	//Cone Maior
	qreal bAngleCone = 75; //angulo de abertura do cone maior
	qreal ang3 = ball_robot.angle(); //angulo da linha que liga bola pro robo
	qreal ang2 = ((int)(btAngle + bAngleCone))%360;
	qreal ang1 = ((int)(btAngle - bAngleCone))%360;

	//Cone Menor
	qreal sAngleCone = 45; //angulo de abertura do cone menor
	qreal ang4 = ((int)(btAngle - sAngleCone))%360;
	qreal ang5 = ((int)(btAngle + sAngleCone))%360;

	//Verificando mudança de estado
	if(inCone){
		if(!( (ang2 > ang1 && ( ang3 < ang2 && ang3 > ang1 ) ) || 
			  (ang2 < ang1 && ( ang3 < ang2 || ang3 > ang1 ) ) )){ //n esta dentro do cone maior

			inCone = false;
		}
	}
	else{
		if( (ang5 > ang4 && ( ang3 < ang5 && ang3 > ang4 ) ) || 
			(ang5 < ang4 && ( ang3 < ang5 || ang3 > ang4 ) ) ){ //esta dentro do cone menor

			inCone = true;
		}
	}

	if(!inCone){
		Point p = ball_target.p2();
		gotoa->setTarget(p.x(), p.y());
		gotoa->step();
	}
	else
		DriveToObject::step();
}

bool DriveToBall::busy()
{
	return DriveToObject::busy();
}
