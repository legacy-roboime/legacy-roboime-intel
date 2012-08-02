#include "GetBall.h"
#include "Goto.h"
#include "Robot.h"
#include "Object.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include <QLineF>
#include "mathutils.h"

//valor carteado
#define CART 82.6

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

GetBall::GetBall(QObject* p, Robot* r, qreal s, qreal orientation)
	: Goto(p, r, r->stage()->ball()->x(), r->stage()->ball()->y(), orientation, s)
{
	speed = s;

}

qreal GetBall::getSpeed()
{
	return speed;
}

bool GetBall::hasBall() {
	static qreal dx, dy, /*d, x, y,*/ bx, by;
	//82.6
	qreal rx, ry, distance;

	//ball position
	bx = stage()->ball()->x();// + stage()->ball()->radius() / 2;
	by = stage()->ball()->y();

	//dribbler effective position
	rx = robot()->x() + cos(robot()->orientation()) * CART;
	ry = robot()->y() + sin(robot()->orientation()) * CART;

	dx = bx - rx;
	dy = by - ry;

	//printf("%f\n",bx);

	distance = sqrt(dx * dx + dy * dy);

	if(distance <= robot()->body().radius() * 0.9  + stage()->ball()->radius() /*&& abs(diffAngle) < M_PI / 90.*/) {
		return true;
	}
	else{
		return false;
	}
}

void GetBall::step()
{
	qreal distance = 300;

	static Stage* stage = this->stage();
	static Robot* robot = this->robot();

	float PI = std::atan(1.0f) * 4.0f;

	qreal GolX = robot->enemyGoal()->x();
	qreal GolY = robot->enemyGoal()->y();
	qreal ballX = stage->ball()->x();
	qreal ballY = stage->ball()->y();
	qreal rx =robot->x();
	qreal ry = robot->y();

	qreal dx = robot->x() - stage->ball()->x();
	qreal dy = robot->y() - stage->ball()->y();

	qreal dist = sqrt(dx*dx + dy*dy);

	QLineF BallGoalLine = QLineF(stage->ball()->x() , stage->ball()->y(), GolX, GolY);
	QLineF linePlayer = QLineF(stage->ball()->x() , stage->ball()->y(), robot->x(), robot->y());
	QLineF PlayerGoalLine = QLineF(GolX , GolY, robot->x(), robot->y());
	QLineF lineGoal = QLineF( stage->ball()->x() , stage->ball()->y() , robot->enemyGoal()->x() , robot->enemyGoal()->y() );

	//Nova implementaçao

	qreal goalWidth = robot->enemyGoal()->width();

	//Linha das traves à bola
	QLineF ballGoalBorderLine1 = QLineF(stage->ball()->x() , stage->ball()->y(), GolX, GolY + goalWidth/2);
	QLineF ballGoalBorderLine2 = QLineF(stage->ball()->x() , stage->ball()->y(), GolX, GolY - goalWidth/2);

	//Linha da bola ao robo
	QLineF playerBallLine = QLineF(robot->x() , robot->y(), ballX, ballY);

	qreal xGBL1 =  stage->ball()->x() - GolX;
	qreal yGBL1 =  stage->ball()->y() - (GolY + goalWidth/2);

	qreal xGBL2 =  stage->ball()->x() - GolX;
	qreal yGBL2 =  stage->ball()->y() - (GolY - goalWidth/2);

	qreal xPBL = robot->x() - ballX;
	qreal yPBL = robot->y() - ballY;


	qreal crossProduct1 = xPBL*yGBL1 - xGBL1 * yPBL;
	qreal crossProduct2 = xPBL*yGBL2 - xGBL2 * yPBL;

	qreal criteria = crossProduct1 * crossProduct2 ;
	bool condition;// = crossProduct1 < 0;//unused value

	if(robot->goal()->x() < 0) condition = rx < ballX;
	else condition = rx > ballX;

	if( criteria < 0 && condition ) { //esta no cone de chute

		QLineF GoalLine = QLineF(robot->enemyGoal()->x(), 0.0, robot->enemyGoal()->x(), 1.0);

		QPointF GoalPos;

		GoalLine.intersect(linePlayer, &GoalPos);

		qreal reducedSpeed = dist<500?(speed/4): speed;
		Goto::setIgnoreBrake();

		if(hasBall()) {
			robot->dribble();
			//emit out();
		}
		else{
			robot->dribble(0);
		}

		Goto::setAll(ballX/*GoalPos.x()*/ , ballY/*GoalPos.y()*/, reducedSpeed);
		Goto::setOrientation(ballX - rx, ballY - ry );
		Goto::step();
	}
	else { //nao esta no cone de chute, move-se para dentro
		
		//if(!condition) {
			//Blocker::step();
		//} else {
        if(condition) {
			qreal ballGoalAngle = BallGoalLine.angle();
			qreal radAngle = ballGoalAngle * PI/180;
			//if(radAngle < 0) radAngle = -1 * radAngle;

			//int side = robot->goal()->x() < 0 ? -1 : 1;//unused
		

			qreal destX = ballX - distance * cos(radAngle);
			qreal destY = ballY + distance * sin(radAngle);

			Goto::setNotIgnoreBrake();
			Goto::setAll(destX , destY, speed);
		
			Goto::setOrientation(ballX - destX, ballY - destY );
			Goto::step();
		}

		robot->dribble(0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*

	static qreal dx, dy, d, x, y, bx, by;
	qreal rx, ry;//, distance;
	//qreal dThreshold = 300.;
	//qreal maxSpeed = 2600.;
	//qreal speedBall;

	bx = stage->ball()->x();// + stage()->ball()->radius() / 2;
	by = stage->ball()->y();
	rx = robot->x();
	ry = robot->y();
	//speedBall = sqrt(pow(stage()->ball()->speed().x(),2) + pow(stage()->ball()->speed().y(),2));
	//printf("velocidade linear bola: %f\n",speedBall);
	//qreal rOrientation = robot()->orientation();

	dx = bx - rx;
	dy = by - ry;
	//distance = sqrt(dx * dx + dy * dy);

	Goto::setOrientation(dx, dy);

	d = robot->body().radius() - CART + stage->ball()->radius();
	x = bx - d * cos(orientation);
	y = by - d * sin(orientation);
	Goto::setPoint(x, y);
	
	//Proportional Control
	//if(distance < dThreshold){
	//	//qreal errorP = -2.*pow(distance/dThreshold,3)+3*pow(distance/dThreshold,2);
	//	//Goto::setSpeed(maxSpeed * errorP);// + speedBall);
	//	//TODO: realizar controle aqui
	//	Move::setSpeeds(dx / robot()->body().radius() / robot()->body().radius(), dy / robot()->body().radius() / robot()->body().radius());//CARTEADO
	//	Move::step();

	//} else {
	//	//Goto::setSpeed(maxSpeed); // + speedBall);
	//	Goto::step();
	//}

	Goto::step();

	//Verifica alinhamento da bola com a frente do robo (sera substituido pelo sensor da bola)
	//QLineF lineTo = QLineF(rx, ry, rx + 1., ry);
	//lineTo.setAngle(rOrientation*180./M_PI);
	//QLineF lineToBall = QLineF(rx, ry, bx, by);
	//qreal diffAngle = lineTo.angleTo(lineToBall) * M_PI / 180.;
	*/
}
