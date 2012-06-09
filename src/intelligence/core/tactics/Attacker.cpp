#include "Attacker.h"
#include "Steer.h"
#include "Robot.h"
#include "Stage.h"
#include "Ball.h"
#include "Goal.h"
#include "Team.h"
#include "Skills.h"
#include "Loops.h"
#include <cmath>
#include <QLineF>

using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

Attacker::Attacker(QObject* p, Robot* r, qreal s)
	: Blocker(p,r,3.14/6,s),
	goto_(new Goto(this, r)),
	kickTo_(new KickTo(this, r)),
	getBall_(new GetBall(this, r, 1000)),
	orbit_(new Loops::Orbit(this,r,0,0,100,1000,1000)),
	move_(new Move(this,r,0,0,0))
{
	speed = s;
	goto_->setSpeed(speed);
	this->pushState(goto_);//this is important
	this->pushState(kickTo_);
	this->pushState(getBall_);
	this->pushState(orbit_);
	this->pushState(move_);

	//skills.append(goto_);//this is important
	//skills.append(kickTo_);
	//skills.append(getBall_);
	//skills.append(orbit_);
	//skills.append(move_);
}


/// Estratégia:
/// Move para a bola, domina e chuta a gol
void Attacker::step()
{
	qreal distance = 300;

	static Stage* stage = this->stage();
	static Robot* robot = this->robot();
	//goto_->step();
	float PI = std::atan(1.0f) * 4.0f;
	// Gol Inimigo
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
	bool condition = crossProduct1 < 0;

	if(robot->goal()->x() < 0) condition = rx < ballX;
	else condition = rx > ballX;

	if( criteria < 0 && condition ) { //esta no cone de chute

		QLineF GoalLine = QLineF(robot->enemyGoal()->x(), 0.0, robot->enemyGoal()->x(), 1.0);

		QPointF GoalPos;

		GoalLine.intersect(linePlayer, &GoalPos);

		qreal reducedSpeed = dist<500?(speed/4): speed;
		goto_->setIgnoreBrake();

		goto_->setAll(GoalPos.x() , GoalPos.y(), reducedSpeed);
		robot->kick(1);
		goto_->setOrientation(ballX - rx, ballY - ry );
		goto_->step();
	}
	//else if(false && criteria < 0 && !condition) {
	//	//move-se perpendicular em direcao ao centro, saindo do do cone
	//	
	//	qreal ballGoalAngle = BallGoalLine.angle();
	//	qreal radAngle = ballGoalAngle * PI/180;
	//	//if(radAngle < 0) radAngle = -1 * radAngle;


	//	qreal destX = ballX - distance * cos(radAngle);
	//	qreal destY = ballY + distance * sin(radAngle);

	//	goto_->setNotIgnoreBrake();
	//	//goto_->setNotIgnoreBall();
	//	goto_->setAll(destX , destY, speed);
	//	
	//	goto_->setOrientation(ballX - destX, ballY - destY );
	//	goto_->step();

	//}
	else { //nao esta no cone de chute, move-se para dentro
		
		if(!condition) {
			Blocker::step();
		}
		else {
			qreal ballGoalAngle = BallGoalLine.angle();
			qreal radAngle = ballGoalAngle * PI/180;
			//if(radAngle < 0) radAngle = -1 * radAngle;

			int side = robot->goal()->x() < 0 ? -1 : 1; 
		

			qreal destX = ballX - distance * cos(radAngle);
			qreal destY = ballY + distance * sin(radAngle);

			goto_->setNotIgnoreBrake();
			goto_->setAll(destX , destY, speed);
		
			goto_->setOrientation(ballX - destX, ballY - destY );
			goto_->step();
		}
	}
	
	
	//qreal dist = 200;
	//qreal angle =BallGoalLine.angle();
	//qreal cx = ballX - (dist * cos( BallGoalLine.angle() * PI/180  ) );
	//qreal cy = ballY + (dist * sin( BallGoalLine.angle() * PI/180  ) );

	//qreal tol = 300;
	//


	//if(distance <=270)
	//{
	//	

	//	

	//		if(getBall_->hasBall()) { //se esta ao alcance da bola, chuta
	//			qreal angle = linePlayer.angle();
	//			qreal speedx = +speed * cos(BallGoalLine.angle() * PI/180);
	//			qreal speedy = +speed * sin(BallGoalLine.angle() * PI/180);
	//			robot->kick(1);
	//			move_->setAll(1000,0,0);
	//			move_->step();

	//			/*
	//			QLineF BallGoalLine = QLineF(stage->ball()->x(), stage->ball()->y(), GolX, GolY);
	//			float PI = std::atan(1.0f) * 4.0f;

	//			qreal AngGraus = BallGoalLine.angle();
	//			qreal AngRad = AngGraus * PI/180;

	//			kickTo_->setAngle(AngRad);
	//			kickTo_->step();
	//			*/
	//	
	//		}
	//		else { //senao se aproxima
	//			qreal speed = 1000;
	//			qreal speedx = speed * cos(linePlayer.angle() * 3.14/180);
	//			qreal speedy = speed * sin(linePlayer.angle() * 3.14/180);
	//			robot->kick(1);
	//			move_->setAll(1000,0,0);
	//			move_->step();
	//		}


	//	
	//}
	//else 
	//{
	//	//getBall_->setOrientation(GolX - robot->x(), GolY - robot->y());
	//	//getBall_->step();
	//	goto_->setIgnoreBall();
	//	goto_->setAll(cx , cy, speed);
	//	
	//	goto_->setOrientation(ballX - cx, ballY - cy );
	//	goto_->step();
	//}


	///*
	//float PI = std::atan(1.0f) * 4.0f;
	//qreal ballX = stage->ball()->x();
	//qreal ballY = stage->ball()->y();
	//qreal dist = 300;
	//qreal angle =BallGoalLine.angle();
	//qreal cx = ballX - (dist * cos( BallGoalLine.angle() * PI/180  ) );
	//qreal cy = ballY + (dist * sin( BallGoalLine.angle() * PI/180  ) );

	//qreal tol = 300;
	//qreal rx =robot->x();
	//qreal ry = robot->y();

	//if( distance <= 350   )
	//{
	//			qreal angle = linePlayer.angle();
	//			qreal speed = 500;
	//			qreal speedx = -speed * cos(BallGoalLine.angle() * PI/180);
	//			qreal speedy = +speed * sin(BallGoalLine.angle() * PI/180);
	//			robot->kick(1);
	//			move_->setAll(speedx,speedy,0);
	//			move_->step();
	//}
	//else {
	//	goto_->setIgnoreBall();
	//	goto_->setAll(cx , cy, 500);
	//	
	//	goto_->setOrientation(GolX - ballX, GolY - ballY );
	//	goto_->step();
	//}
	////}
	//	
	//	*/
	//

	//

}


//void Attacker::stepConducao() {
//
//	qreal distance = 100;
//
//	Stage* stage = this->stage();
//	Robot* robot = this->robot();
//	goto_->step();
//	float PI = std::atan(1.0f) * 4.0f;
//	// Gol Inimigo
//	qreal GolX = robot->enemyGoal()->x();
//	qreal GolY = robot->enemyGoal()->y();
//	qreal ballX = stage->ball()->x();
//	qreal ballY = stage->ball()->y();
//	qreal rx =robot->x();
//	qreal ry = robot->y();
//
//	qreal dx = robot->x() - stage->ball()->x();
//	qreal dy = robot->y() - stage->ball()->y();
//
//	qreal dist = sqrt(dx*dx + dy*dy);
//
//	
//
//	QLineF BallGoalLine = QLineF(stage->ball()->x() , stage->ball()->y(), GolX, GolY);
//	QLineF linePlayer = QLineF(stage->ball()->x() , stage->ball()->y(), robot->x(), robot->y());
//	QLineF PlayerGoalLine = QLineF(GolX , GolY, robot->x(), robot->y());
//	QLineF lineGoal = QLineF( stage->ball()->x() , stage->ball()->y() , robot->enemyGoal()->x() , robot->enemyGoal()->y() );
//
//
//	//Nova implementaçao
//
//	qreal goalWidth = robot->enemyGoal()->width();
//
//	//Linha das traves à bola
//	QLineF ballGoalBorderLine1 = QLineF(stage->ball()->x() , stage->ball()->y(), GolX, GolY + goalWidth/2);
//	QLineF ballGoalBorderLine2 = QLineF(stage->ball()->x() , stage->ball()->y(), GolX, GolY - goalWidth/2);
//
//	//Linha da bola ao robo
//	QLineF playerBallLine = QLineF(robot->x() , robot->y(), ballX, ballY);
//
//	qreal xGBL1 =  stage->ball()->x() - GolX;
//	qreal yGBL1 =  stage->ball()->y() - (GolY + goalWidth/2);
//
//	qreal xGBL2 =  stage->ball()->x() - GolX;
//	qreal yGBL2 =  stage->ball()->y() - (GolY - goalWidth/2);
//
//	qreal xPBL = robot->x() - ballX;
//	qreal yPBL = robot->y() - ballY;
//
//
//	qreal crossProduct1 = xPBL*yGBL1 - xGBL1 * yPBL;
//	qreal crossProduct2 = xPBL*yGBL2 - xGBL2 * yPBL;
//
//	qreal criteria = crossProduct1 * crossProduct2 ;
//
//	if( criteria < 0 ) { //esta no cone de chute
//
//		
//		QLineF GoalLine = QLineF(robot->enemyGoal()->x(), 0.0, robot->enemyGoal()->x(), 1.0);
//
//		QPointF GoalPos;
//
//		GoalLine.intersect(linePlayer, &GoalPos);
//
//		
//		goto_->setIgnoreBrake();
//		goto_->setIgnoreBall();
//
//		goto_->setAll(GoalPos.x() , GoalPos.y(), 500);
//		goto_->setOrientation(ballX - rx, ballY - ry );
//		goto_->step();
//
//
//	}
//	/*else if(criteria < 0 && crossProduct1 > 0) {
//		//move-se perpendicular em direcao ao centro, saindo do do cone
//		
//		qreal ballGoalAngle = BallGoalLine.angle();
//		qreal radAngle = ballGoalAngle * PI/180;
//		//if(radAngle < 0) radAngle = -1 * radAngle;
//
//
//		qreal destX = ballX - distance * cos(radAngle);
//		qreal destY = ballY + distance * sin(radAngle);
//
//		goto_->setNotIgnoreBrake();
//		goto_->setNotIgnoreBall();
//		goto_->setAll(destX , destY, speed);
//		
//		goto_->setOrientation(ballX - destX, ballY - destY );
//		goto_->step();
//
//	}*/
//	else { //nao esta no cone de chute, move-se para dentro
//		
//		qreal ballGoalAngle = BallGoalLine.angle();
//		qreal radAngle = ballGoalAngle * PI/180;
//		//if(radAngle < 0) radAngle = -1 * radAngle;
//
//		int side = robot->goal()->x() < 0 ? -1 : 1; 
//		
//
//		qreal destX = ballX - distance * cos(radAngle);
//		qreal destY = ballY + distance * sin(radAngle);
//
//		goto_->setNotIgnoreBrake();
//		goto_->setIgnoreBall();
//		goto_->setAll(destX , destY, speed);
//		
//		goto_->setOrientation(ballX - destX, ballY - destY );
//		goto_->step();
//
//	}
//
//}