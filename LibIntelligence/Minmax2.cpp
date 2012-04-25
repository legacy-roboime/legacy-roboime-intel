#include "Minmax2.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"
#include <QLineF>

#define LOGGING

using namespace LibIntelligence::Plays;
using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

Minmax2::Minmax2(QObject *parent, Team* team ,Stage* stage, int depth, float alpha, float beta, qreal speed):
Play(parent,team,stage),
	log("C:\\Users\\Bill\\Desktop\\log.dat"),
	depth_(depth),
	alpha_(alpha),
	beta_(beta),
	speed_(speed)
{
	log.open(QIODevice::WriteOnly);

	s = sstate_alloc();

	for(int i=0; i < team->size(); i++)
		_max_skills.push_back( new Goto(this, team->at(i)) );
}


Minmax2::~Minmax2()
{
	for(int i=0; i<_max_skills.size(); i++)
		delete _max_skills.at(i);
	log.close();
}


void Minmax2::step()
{
	update_soccer_state();
	//cout << s->red_ball_owner << endl;
	//cout << s->blue_ball_owner << endl;
	minimax_getMaxValue( *s, depth_, alpha_, beta_ );
	//for(int i=0; i<5; i++)
	//	printf("DENTRO: %d %f %f\n", i, best_action.move[i].x, best_action.move[i].y);
	//for(int i=0; i < team_->size(); i++)
	//	cout << best_action.move[i].x << endl;
	saction_act();
}


void Minmax2::saction_act()
{
	Ball* ball = stage_->ball();

	for(int i=0; i < team_->size(); i++){
		Vector2* pos = &best_action.move[i];
		Robot* robot = team_->at(i);

		_max_skills.at(i)->setPoint(robot->x() + pos->x, robot->y() + pos->y);

		//cout << "FORA: " << team_->at(i)->id() << " " << pos->x << " " << pos->y << endl;

		if( team_->at(i)->id() != s->red_ball_owner ){
			QLineF line = QLineF(robot->x(), robot->y(), ball->x(), ball->y());
			qreal orientation = - line.angle() * PI / 180; //convenção sentido horario para classe QLineF

			_max_skills.at(i)->setSpeed(s->red_speed);
			_max_skills.at(i)->setOrientation(orientation);
			robot->dribble();
		}
		else{
			if( best_action.has_kicked ){
				QLineF line = QLineF(robot->x(), robot->y(), best_action.kick_point.x, best_action.kick_point.y);
				qreal orientation = - line.angle() * PI / 180; //convenção sentido horario para classe QLineF

				_max_skills.at(i)->setSpeed(s->red_dribble_speed);
				_max_skills.at(i)->setOrientation(orientation);
				robot->kick();
				robot->dribble();
			}
			else if( best_action.has_passed ){
				QLineF line = QLineF(robot->x(), robot->y(), best_action.kick_point.x, best_action.kick_point.y);
				qreal orientation = - line.angle() * PI / 180; //convenção sentido horario para classe QLineF

				_max_skills.at(i)->setSpeed(s->red_dribble_speed);
				_max_skills.at(i)->setOrientation(orientation);
				robot->kick(s->red_pass_speed);
				robot->dribble();
			}
			else{
				Goal* enemyGoal = robot->enemyGoal();
				QLineF line = QLineF(robot->x(), robot->y(), /*ball*/enemyGoal->x(), /*ball*/enemyGoal->y());
				qreal orientation = - line.angle() * PI / 180; //convenção sentido horario para classe QLineF

				_max_skills.at(i)->setSpeed(s->red_dribble_speed);
				_max_skills.at(i)->setOrientation(orientation);
				robot->dribble();
			}
		}

		robot->dribble();
		_max_skills.at(i)->step();
	}
}


void Minmax2::update_soccer_state()
{
	Ball* ball = stage_->ball();
	s->ball.x = ball->x();
	s->ball.y = ball->y();
	s->ball_vel.x = ball->speedX();
	s->ball_vel.y = ball->speedY();

	//as cores nessa play (red e blue) identificam se eh o nosso time (nosso time eh o red red)
	//ou nao, nao esta relacionado com a cor de time da inteligencia (yellow e blue)
	Team* blueTeam = team_->enemyTeam();
	for(int i=0; i< blueTeam->size(); i++){
		Robot* robot = blueTeam->at(i);
		s->blue[i].x = robot->x();
		s->blue[i].y = robot->y();
	}

	Team* redTeam = team_; 
	for(int i=0; i< redTeam->size(); i++){
		Robot* robot = redTeam->at(i);
		s->red[i].x = robot->x();
		s->red[i].y = robot->y();
	}

	//DUVIDA: EH EXCLUSIVO RED E BLUE (SE EXISTE UM RED_BALL_OWNER PODE EXISTIR UM BLUE_BALL_OWNER AO MESMO TEMPO !!?) !??!
	s->red_ball_owner = ballOwner(true);
	s->blue_ball_owner = ballOwner(false);

	//TODO: atualizar os campos (parâmetros constantes) da struct Soccer State de s, 
	//para que nao haja mais de uma fonte de parâmetros constantes, essa implementacao 
	//nao precisa ser executada em cada update soccer state (mas nao pode colocar no construtor 
	//pq a informacao nao chegou ainda para alguns parametros)
	//goal scored e gol receive nao precisa atualizar, pois eh variavel auxiliar (interna) dessa inteligencia
}


float Minmax2::minimax_getMaxValue(SoccerState s, int depth, float alpha, float beta )
{
	int i;
	float aux; 
	SoccerState saux;
	SoccerAction action;

	if( depth == 0 ){
		saux = s;
		//  sstate_red_kick_to_goal(&saux); 
		return sstate_evaluate(&saux);
	}
	for( i = 0; i < MAX_NPLAYS; i++ ){
		saux = s;
		action = minimax_expandMax( &saux, i, depth );
		if( alpha < (aux = minimax_getMinValue( saux, depth-1, alpha, beta )) ){
			alpha = aux;
			if( depth == MINIMAX_MAX_LEVEL ){
				best_action = action;
			}
			if( action.has_kicked )
				break;
		}
		if( beta <= alpha )
			break;
	}
	return alpha;
}


float Minmax2::minimax_getMinValue(SoccerState s, int depth, float alpha, float beta )
{
	int i;
	float aux;
	SoccerState saux;

	if( depth == 0 ){
		saux = s;
		sstate_blue_kick_to_goal(&saux); 
		return sstate_evaluate(&saux);
	}
	for( i = 0; i < MIN_NPLAYS; i++ ){
		saux = s;
		minimax_expandMin( &saux, i, depth );
		if( beta > (aux = minimax_getMaxValue( saux, depth-1, alpha, beta )) ){
			beta = aux;
			if( (beta <= (-MAX_FLOAT + EPS)) )
				break;
		} 
		if( beta <= alpha )
			break;
	}
	return beta;
}


SoccerAction Minmax2::minimax_expandMax( SoccerState *s, int i, int depth )
{
	float recv_radius = 400., move_radius = 1000.;
	SoccerAction action = saction_make(-1);

	if( s->red_ball_owner >= 0 ){
		switch( i ){
		case 0: action = sstate_red_kick_to_goal(s); break;
		//case 1: action = sstate_red_pass(s,0, recv_radius ); break;
		//case 3: action = sstate_red_pass(s,1, recv_radius ); break;
		//case 4: action = sstate_red_pass(s,2, recv_radius ); break;
		//case 5: action = sstate_red_pass(s,3, recv_radius ); break;
		//case 6: action = sstate_red_pass(s,4, recv_radius ); break; 
		//case 7: action = sstate_red_pass(s,5, recv_radius ); break; 
		}
	}
	else{
		switch( i ){
		case 0: action = sstate_red_get_ball(s); break;
		//case 8: action = sstate_red_receive_ball(s,0); break; 
		//case 9: action = sstate_red_receive_ball(s,1); break;
		//case 10: action = sstate_red_receive_ball(s,2); break;
		//case 11: action = sstate_red_receive_ball(s,3); break;
		//case 12: action = sstate_red_receive_ball(s,4); break;
		//case 13: action = sstate_red_receive_ball(s,5); break;
		}
	}

	//ESSE CODIGO NAO DEVERIA ESTAR DENTRO DO ELSE ACIMA E COM ELSE IF ALTERNADOS ?!?!?!?!?
	if( (i > 13) && (i < 23 ) )
		action = sstate_red_move(s, move_radius );

	if( (i>= 23) && ( i < 43) )
		action = sstate_red_move(s, (2./3)*move_radius );

	if( (i >= 43) && ( i < 50) )
		action = sstate_red_move(s, (1./3)*move_radius );

	return action;
}


void Minmax2::minimax_expandMin( SoccerState *s, int i, int depth )
{
	float recv_radius, move_radius;

	recv_radius = 400;
	move_radius = 1000;


	if( s->blue_ball_owner >= 0 ){
		switch( i ){
		case 0: sstate_blue_kick_to_goal(s); break;
		case 2: sstate_blue_pass(s,0, recv_radius ); break;
		case 3: sstate_blue_pass(s,1, recv_radius ); break;
		case 4: sstate_blue_pass(s,2, recv_radius ); break;
		case 5: sstate_blue_pass(s,3, recv_radius ); break;
		case 6: sstate_blue_pass(s,4, recv_radius ); break; 
		case 7: sstate_blue_pass(s,5, recv_radius ); break; 
		}
	}
	else{
		switch( i ){ 
		case 1: sstate_blue_get_ball(s); break;
		case 8: sstate_blue_receive_ball(s,0); break;
		case 9: sstate_blue_receive_ball(s,1); break;
		case 10: sstate_blue_receive_ball(s,2); break;
		case 11: sstate_blue_receive_ball(s,3); break;
		case 12: sstate_blue_receive_ball(s,4); break;
		case 13: sstate_blue_receive_ball(s,5); break;
		}
	}

	if( (i > 13) && (i < 23 ) )
		sstate_blue_move(s, move_radius );

	if( (i>= 23) && ( i < 43) )
		sstate_blue_move(s, (2./3)*move_radius );

	if( (i >= 43) && ( i < 50) )
		sstate_blue_move(s, (1./3)*move_radius );
}

int Minmax2::ballOwner(bool us)
{
	Team* team;
	qreal lErrorA = MAX_DOUBLE;
	qreal lErrorD = MAX_DOUBLE;
	int id = -1;
	if(us)
		team = team_;
	else
		team_->enemyTeam();

	for(int i=0; i<team->size(); i++){
		Robot* robot = team->at(i);
		Ball* ball = stage_->ball();

		qreal x = robot->x();
		qreal y = robot->y();
		qreal xL = ball->x();
		qreal yL = ball->y();
		qreal errorX = xL - x;
		qreal errorY = yL - y;
		qreal errorD = sqrt(errorX*errorX + errorY*errorY);

		qreal angle1 = robot->orientation();
		QLineF line = QLineF(x, y, xL, yL); 
		qreal angle2 = PITIMES2 - line.angle() * PI / 180;

		qreal errorA = abs(angle2 - angle1); //angle1 e angle2 entre 0 e 2PI
		if( errorA > PI )
			errorA = PITIMES2 - errorA;

		//printf("%f %f\n", errorA * 180. / PI, errorD);

		if(errorD < 150. && errorA < 5 * PI/180. && errorD < lErrorD && errorA < lErrorA){
			id = robot->id();
			lErrorA = errorA;
			lErrorD = errorD;
		}
		else
			continue;
	}

	return id;
}