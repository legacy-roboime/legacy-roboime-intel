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
	speed_(speed),
	init(false)
{
	log.open(QIODevice::WriteOnly);

	soccer_env_init();
	s = sstate_alloc();

	if(team->color() == TeamColor::BLUE)
		soccer_env()->left_red_side = Stage::isLeftSideBlueGoal();
	else
		soccer_env()->left_red_side = !Stage::isLeftSideBlueGoal();

	envReal = *soccer_env();
	changeSEnvMeasure(&envReal, 1000.);

	for(int i=0; i < team->size(); i++)
		_max_skills.push_back( new Goto(this, team->at(i)) );

	//team->at(0) eh soh pra inicializar depois usamos setRobot
	attacker = new AttackerMinMax2(this, team->at(0), envReal.red_speed, 
								   envReal.red_dribble_speed, 
								   envReal.red_pass_speed); 
}

Minmax2::~Minmax2()
{
	for(int i=0; i<_max_skills.size(); i++)
		delete _max_skills.at(i);

	delete attacker;
	log.close();
}

void Minmax2::changeSStateMeasure(SoccerState *s, double scale)
{
	s->ball = v2_scale(scale, s->ball);
	s->ball_vel = v2_scale(scale, s->ball_vel);
	for(int i=0; i<NPLAYERS; i++){
		s->blue[i] = v2_scale(scale, s->blue[i]);
		s->red[i] = v2_scale(scale, s->red[i]);
	}
}

void Minmax2::changeSActionMeasure(SoccerAction *a, double scale)
{
	a->kick_point = v2_scale(scale, a->kick_point);
	a->passer_pos = v2_scale(scale, a->passer_pos);
	for(int i=0; i<NPLAYERS; i++){
		a->move[i] = v2_scale(scale, a->move[i]);
		a->pos[i] = v2_scale(scale, a->pos[i]);
	}
}

void Minmax2::changeSEnvMeasure(SoccerEnvironment *s, double scale)
{
	s->red_speed *= scale;
	s->blue_speed *= scale;
	s->red_dribble_speed *= scale;
	s->blue_dribble_speed *= scale;
	s->red_pass_speed *= scale;
	s->blue_pass_speed *= scale;
	s->red_move_radius *= scale;
	s->blue_move_radius *= scale;
	s->red_recv_radius *= scale;
	s->blue_recv_radius *= scale;
	s->robot_radius *= scale;
	s->goal_size *= scale;
	s->max_red_kick_dist *= scale;
	s->max_blue_kick_dist *= scale;
	s->hfield_h *= scale;
	s->hfield_w *= scale;
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

void Minmax2::update_soccer_state()
{
	Ball* ball = stage_->ball();
	s->ball.x = ball->x();
	s->ball.y = ball->y();
	s->ball_vel.x = 0;//ball->speedX();
	s->ball_vel.y = 0;//ball->speedY();

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

void Minmax2::step()
{
	static SoccerState saux;
	update_soccer_state();

	saux = *s;

	minimax_use_next_red_robot();
	minimax_use_next_blue_robot();

	changeSStateMeasure(&saux, 0.001);

#ifdef SOCCER_DEBUG
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	int winWidth = 1200;
	int winHeight = 400;
	glViewport(0, 0, winWidth/2., winHeight );   
	soccer_redraw( &saux );
#endif

	if(!init){
		minimax_init(&saux);
		init = true;
	}

	minimax_play( &saux, depth_ );

	red_action = *minimax_get_best_red_action();
	blue_action = *minimax_get_best_blue_action();

#ifdef SOCCER_DEBUG
	saction_blue_act( &saux, &blue_action );
	saction_red_act( &saux, &red_action );
	glViewport(winWidth /2., 0, winWidth /2., winHeight ); 
	soccer_redraw( &saux ); 
	glutSwapBuffers();
	//usleep(800000);
#endif

	changeSActionMeasure(&red_action, 1000.);
	changeSActionMeasure(&blue_action, 1000.);

	act();
}

void Minmax2::act()
{
	Ball* ball = stage_->ball();
	int idClosest = stage_->getClosestPlayerToBall(team_)->id();

	for(int i=0; i < team_->size(); i++){
		Vector2* pos = &red_action.move[i];
		Robot* robot = team_->at(i);

		if( idClosest != robot->id() ){
			QLineF line = QLineF(robot->x(), robot->y(), ball->x(), ball->y());
			qreal orientation = PITIMES2 - line.angle() * PI / 180; //convenção sentido horario para classe QLineF

			_max_skills.at(i)->setPoint(pos->x, pos->y);
			_max_skills.at(i)->setSpeed(envReal.red_speed);
			_max_skills.at(i)->setOrientation(orientation);
			_max_skills.at(i)->step();
		}
		else{
			//DEBUG
			QLineF l = QLineF(0, 0, pos->x, pos->y);
			qreal angle = -l.angle() + 360; //convenção sentido horario para classe QLineF
			cout << angle << endl;
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
			attacker->updateSoccerAction(red_action.has_kicked, red_action.has_passed, false,//red_action.type == get_ball,
				red_action.kick_point.x, red_action.kick_point.y, 
				pos->x, pos->y);
			attacker->step();
		}
	}
}