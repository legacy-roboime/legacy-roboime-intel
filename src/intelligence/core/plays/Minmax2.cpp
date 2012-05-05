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

	best_action = saction_make(-1);
	prev_best_action = saction_make(-1);
	s = sstate_alloc();
	soccer_env_init();

	if(team->color() == TeamColor::BLUE)
		soccer_env()->left_red_side = Stage::isLeftSideBlueGoal();
	else
		soccer_env()->left_red_side = !Stage::isLeftSideBlueGoal();

	for(int i=0; i < team->size(); i++)
		_max_skills.push_back( new Goto(this, team->at(i)) );

	attacker = new AttackerMinMax2(this, team->at(0), soccer_env()->red_speed, 
								   soccer_env()->red_dribble_speed, 
								   soccer_env()->red_pass_speed); //team->at(0) eh soh pra inicializar depois usamos setRobot
}


Minmax2::~Minmax2()
{
	for(int i=0; i<_max_skills.size(); i++)
		delete _max_skills.at(i);

	delete attacker;
	log.close();
}


void Minmax2::step()
{
	update_soccer_state();
	best_action = saction_make(-1);
	minimax_getMaxValue( *s, depth_, alpha_, beta_ );
	act();
	//cout << s->red_ball_owner << " " << s->blue_ball_owner << endl;
	//cout << teste << endl;
	//if(best_action.has_kicked){
	//	cout << best_action.kick_point.x << " " << best_action.kick_point.y << endl;
	//}
	prev_best_action = best_action;
}


void Minmax2::act()
{
	Ball* ball = stage_->ball();
	int idClosest = stage_->getClosestPlayerToBall(team_)->id();

	for(int i=0; i < team_->size(); i++){
		Vector2* pos = &best_action.move[i];
		Robot* robot = team_->at(i);

		if( idClosest != robot->id() ){
			QLineF line = QLineF(robot->x(), robot->y(), ball->x(), ball->y());
			qreal orientation = PITIMES2 - line.angle() * PI / 180; //convenção sentido horario para classe QLineF

			_max_skills.at(i)->setPoint(robot->x() + pos->x, robot->y() + pos->y);
			_max_skills.at(i)->setSpeed(soccer_env()->red_speed);
			_max_skills.at(i)->setOrientation(orientation);
			_max_skills.at(i)->step();
		}
		else{
			//DEBUG
			//QLineF l = QLineF(0, 0, pos->x, pos->y);
			//qreal angle = -l.angle() + 360; //convenção sentido horario para classe QLineF
			//if(teste > 12) 
			//	cout << "MOVE " << angle << endl;
			//else if(teste == 0 && best_action.has_kicked)
			//	cout << "GOALKICK " << angle << endl;
			//else if(teste == 0)
			//	cout << "GETBALL " << angle << endl;
			//else if(teste > 7)
			//	cout << "RECV_BALL " << angle << endl;
			//else
			//	cout << "PASS_BALL " << angle << endl;

			attacker->setRobot(robot);
			attacker->updateSoccerAction(best_action.has_kicked, best_action.has_passed, best_action.get_ball,
										 best_action.kick_point.x, best_action.kick_point.y, 
										 robot->x() + pos->x, robot->y() + pos->y);
			attacker->step();
		}
	}
}

void Minmax2::saction_act( SoccerState *s, SoccerAction *sa )
{
	int i, owner;

	if( sa->has_kicked ){
		printf( "# Goal Scored!!\n" );
		sstate_restart_game_pos(s);
		return; 
	}

	for( i = 0; i < NPLAYERS; i++ ) {
		s->red[i] = v2_add( s->red[i], sa->move[i] );
		//  if( v2_sqrnorm( sa->move[i] ) > EPS )
		//    printf( "# Robot %i moved\n", i );
	} 

	if( sa->has_passed )
		printf("# Pass %i -> %i\n", s->red_ball_owner,
		sa->ball_owner );

	if( (owner = sa->ball_owner) >= 0 ){
		if(s->red_ball_owner == -1) 
			printf( "# red %i get ball\n", sa->ball_owner );
		s->ball = s->red[owner]; 
		s->red_ball_owner = owner;
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

	ballOwner();

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
		sstate_red_kick_to_goal(&saux); 
		return sstate_evaluate(&saux);
	}
	for( i = 0; i < MAX_NPLAYS; i++ ){
		saux = s;
		action = minimax_expandMax( &saux, i, depth );
		if( alpha < (aux = minimax_getMinValue( saux, depth-1, alpha, beta )) ){
			alpha = aux;
			if( depth == MINIMAX_MAX_LEVEL ){
				best_action = action;
				teste = i;
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
		//case 2: action = sstate_red_pass(s,0, recv_radius ); break;
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

	if( (i > 13) && (i < 23 ) )
		action = sstate_red_move(s, move_radius );

	if( (i>= 23) && ( i < 43) )
		action = sstate_red_move(s, (2./3)*move_radius );

	if( (i >= 43) && ( i < 50) )
		action = sstate_red_move(s, (1./3)*move_radius );

	if( i == 50 ){
		saction_act( s, &prev_best_action );
		action = prev_best_action;
	}

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
		}
	}

	if( (i > 13) && (i < 23 ) )
		sstate_blue_move(s, move_radius );

	if( (i>= 23) && ( i < 43) )
		sstate_blue_move(s, (2./3)*move_radius );

	if( (i >= 43) && ( i < 50) )
		sstate_blue_move(s, (1./3)*move_radius );
}

void Minmax2::ballOwner()
{
	//TODO: criar a faixa de transição do estado owner para a area entre dois circulos
	Ball* ball = stage_->ball();
	s->red_ball_owner = -1;
	s->blue_ball_owner = -1;
	Robot* mRobot = stage_->getClosestPlayerToBall(team_);
	Robot* tRobot = stage_->getClosestPlayerToBall(team_->enemyTeam());
	qreal mDist = (mRobot->distance(ball)).module();
	qreal tDist = (tRobot->distance(ball)).module();
	if(mDist <= tDist){
		if(mDist < attacker->minDist()){
			s->red_ball_owner = mRobot->id();
			s->blue_ball_owner = -1;
		}
	}
	else{
		if(tDist < attacker->minDist()){
			s->blue_ball_owner = tRobot->id();
			s->red_ball_owner = -1;
		}
	}
}