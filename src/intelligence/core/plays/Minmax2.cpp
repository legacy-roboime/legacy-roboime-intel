#include "Minmax2.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"
#include <QLineF>
#include "GotoTactic.h"

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
	//log.open(QIODevice::WriteOnly);

	soccer_env_init();
	s = sstate_alloc();

	if(team->color() == TeamColor::BLUE && Stage::isLeftSideBlueGoal())
		soccer_env_red_side( LEFT );
	else if (team->color() == TeamColor::YELLOW && !Stage::isLeftSideBlueGoal())
		soccer_env_red_side( LEFT );
	else
		soccer_env_red_side( RIGHT );

	envReal = *soccer_env();
	changeSEnvMeasure(&envReal, 1000.);

	//for(int i=0; i < team->size(); i++)
	//	_max_skills.push_back( new Goto(this, team->at(i)) );

	//goto_ = new Goto(this, team->at(2));

	for(int i=0; i<5; i++)
		player_[i] = new GotoTactic(this, team->at(i)); 

	attacker = new AttackerMinMax2(this, team->at(0), envReal.red_speed, 
								   envReal.red_dribble_speed, 
								   envReal.red_pass_speed); 
}

Minmax2::~Minmax2()
{
	for(int i=0; i<_max_skills.size(); i++)
		delete _max_skills.at(i);

	delete attacker;
	//log.close();
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
		if(mDist < MIN_DIST){
			s->red_ball_owner = mRobot->id();
			s->blue_ball_owner = -1;
		}
	}
	else{
		if(tDist < MIN_DIST){
			s->blue_ball_owner = tRobot->id();
			s->red_ball_owner = -1;
		}
	}
}

void Minmax2::update_soccer_state()
{
	Team* redTeam; 
	Team* blueTeam;
	redTeam = team_;
	blueTeam = team_->enemyTeam();

	Ball* ball = stage_->ball();
	s->ball.x = ball->x();
	s->ball.y = ball->y();
	s->ball_vel.x = 0;//ball->speedX();
	s->ball_vel.y = 0;//ball->speedY();

	//as cores nessa play (red e blue) identificam se eh o nosso time (nosso time eh o red)
	//ou nao, nao esta relacionado com a cor de time da inteligencia (yellow e blue)
	for(int i=0; i< blueTeam->size(); i++){
		Robot* robot = blueTeam->at(i);
		s->blue[i].x = robot->x();
		s->blue[i].y = robot->y();
	}
	
	for(int i=0; i< redTeam->size(); i++){
		Robot* robot = redTeam->at(i);
		s->red[i].x = robot->x();
		s->red[i].y = robot->y();
	}

	ballOwner();
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

	act(red_action, team_);
	//act(blue_action, team_->enemyTeam());
}

void Minmax2::act(SoccerAction& action, Team* team)
{
	Ball* ball = stage_->ball();
	int idClosest = stage_->getClosestPlayerToBall(team)->id();

#ifdef SOCCER_DEBUG
	if(action.type == kick_to_goal)
		cout << "Kick To Goal" << endl;
	else if(action.type == pass)
		cout << "Pass" << endl;
	else if(action.type == actions::get_ball)
		cout << "Get Ball" << endl;
	else if(action.type == actions::move)
		cout << "Move" << endl;
	else if(action.type == actions::receive_ball)
		cout << "Receive Ball" << endl;
	else if(action.type == actions::null_action)
		cout << "Null action" << endl;
	else
		cout << "nenhum nem outro" << endl;
#endif

	for(int i=0; i < team->size(); i++){
		Vector2* pos = &action.move[i];
		Robot* robot = team->at(i);

		if( idClosest == robot->id() ){
			//cout << "SKILL: " << attacker->getCurrentState()->objectName().toStdString() << endl;

			//DEBUG
			//QLineF l = QLineF(0, 0, pos->x, pos->y);
			//qreal angle = 360 - l.angle(); //convenção sentido horario para classe QLineF
			//qreal orientation = robot->orientation() * 180 / M_PI;
			//static qreal lastOr = angle;//orientation;
			//cout << abs(angle - lastOr) << endl;
			//cout << pos->x << " " << pos->y << endl;

			attacker->setRobot(robot);// = new AttackerMinMax2(this, robot, envReal.red_speed, envReal.red_dribble_speed, envReal.red_pass_speed);
			attacker->updateSoccerAction(action.type, action.kick_point, *pos);
			attacker->step();
			
			//lastOr = angle;
		}
		else{
			QLineF line = QLineF(robot->x(), robot->y(), ball->x(), ball->y());
			qreal orientation = PITIMES2 - line.angle() * PI / 180; //convenção sentido horario para classe QLineF

			//_max_skills.at(i)->setPoint(pos->x, pos->y);
			//_max_skills.at(i)->setSpeed(envReal.red_speed);
			//_max_skills.at(i)->setOrientation(orientation);
			//_max_skills.at(i)->step();

			//cout << pos->x << " " << pos->y << endl;
			//((GotoTactic*)player_[i])->setRobot(robot);
			Goto* g = ((GotoTactic*)player_[i])->goto_;
			g->setAllowDefenseArea();
			g->setPoint(pos->x, pos->y);
			g->setSpeed(envReal.red_speed);
			g->setOrientation(orientation);
			player_[i]->step();
		}
	}
}