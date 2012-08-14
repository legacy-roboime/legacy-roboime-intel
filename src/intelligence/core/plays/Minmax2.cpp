#include "Minmax2.h"
#include "Goal.h"
#include "Ball.h"
#include "Robot.h"
#include "GotoTactic.h"
#include "config.h"
#include <QTime>

#define LOGGING

using namespace LibIntelligence::Plays;
using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

Minmax2::Minmax2(QObject *parent, Team* team ,Stage* stage, qreal speed, int depth, float alpha, float beta)
	: Play(parent, team,stage),
	log("C:\\Users\\Bill\\Desktop\\log.dat"),
	speed_(speed),
	depth_(depth),
	alpha_(alpha),
	beta_(beta),
	init(false)
{
	//log.open(QIODevice::WriteOnly);

	soccer_env_init();
	s = sstate_alloc();
	sL = sstate_alloc();

	red_action = saction_red_make(s);
	blue_action = saction_blue_make(s);

	if(team->color() == BLUE && stage->isLeftSideBlueGoal())
		soccer_env_red_side( LEFT );
	else if (team->color() == YELLOW && !stage->isLeftSideBlueGoal())
		soccer_env_red_side( LEFT );
	else
		soccer_env_red_side( RIGHT );

	envReal = *soccer_env();
	changeSEnvMeasure(&envReal, 1000.);

	//for(int i=0; i < team->size(); i++)
	//	_max_skills.push_back( new Goto(this, team->at(i)) );

	//goto_ = new Goto(this, team->at(2));

	for(int i=0; i<NPLAYERS; i++)
		player_[i] = new GotoTactic((Play *)this, team->at(i)); 

	attacker = new AttackerMinMax2(this, team->at(0), speed_, 
								   speed_, 
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
	qreal orientation = mRobot->orientation();
	qreal dist = 0;//70;
	QPointF mDribbler = QPointF(mRobot->x() + cos(orientation)*dist, mRobot->y() + sin(orientation)*dist);
	orientation = tRobot->orientation();
	QPointF tDribbler = QPointF(tRobot->x() + cos(orientation)*dist, tRobot->y() + sin(orientation)*dist);
	qreal mDist = QVector2D(mDribbler - *ball).length();
	qreal tDist = QVector2D(tDribbler - *ball).length();
	if(mDist <= tDist){
		if(mDist  < MIN_DIST){
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
	s->ball_vel.x = ball->speed().x();
	s->ball_vel.y = ball->speed().y();

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

void Minmax2::run()
{
	while(true) {
#ifdef MINMAX2_DELAY
		QTime time1 = QTime::currentTime();
#endif
#ifdef SOCCER_DEBUG
		statemutex.lock();
#endif
		update_soccer_state();

		//minimax_use_next_red_robot();
		//minimax_use_next_blue_robot();

		changeSStateMeasure(s, 0.001);

		if(!init){
			minimax_init(s);
			init = true;
		}

//#ifdef SOCCER_DEBUG
		*sL = *s;
//#endif

		minimax_play( sL, depth_ );

		mutex.lock();
		red_action = *minimax_get_best_red_action();
		blue_action = *minimax_get_best_blue_action();

		changeSActionMeasure(&red_action, 1000.);
		changeSActionMeasure(&blue_action, 1000.);
		mutex.unlock();

#ifdef SOCCER_DEBUG
		changeSStateMeasure(s, 1000.);
		statemutex.unlock();
#endif
#ifdef MINMAX2_DELAY
		QTime time2 = QTime::currentTime();
		double mSec = (time2.minute() * 60 * 1000 + time2.second() * 1000 + time2.msec()) - (time1.minute() * 60 * 1000 + time1.second() * 1000 + time1.msec());
		cout << "Minmax delay: " << mSec << endl;
#endif
	}
}

void Minmax2::drawOpenGL()
{
	statemutex.lock();
	changeSStateMeasure(s, 0.001);
	changeSActionMeasure(&red_action, .001);
	changeSActionMeasure(&blue_action, .001);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	int winWidth = 1200;
	int winHeight = 400;
	glViewport(0, 0, winWidth/2., winHeight );   
	soccer_redraw( s );

	saction_blue_act( sL, &blue_action );
	saction_red_act( sL, &red_action );
	glViewport(winWidth /2., 0, winWidth /2., winHeight ); 
	soccer_redraw( sL ); 
	glutSwapBuffers();

	changeSStateMeasure(s, 1000.);
	changeSActionMeasure(&red_action, 1000.);
	changeSActionMeasure(&blue_action, 1000.);
	statemutex.unlock();
}

void Minmax2::step()
{
	mutex.lock();
	act(red_action, team_);
	//act(blue_action, team_->enemyTeam());
	mutex.unlock();
}

void Minmax2::act(SoccerAction& action, Team* team)
{
	Ball* ball = stage_->ball();
	int idClosest = stage_->getClosestPlayerToBall(team)->id();

#ifdef SOCCER_ACTION
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
	else if(action.type == actions::blocker)
		cout << "Blocker" << endl;
	else if(action.type == actions::move_table)
		cout << "Move table" << endl;
	else
		cout << "nenhum nem outro" << endl;
#endif

	for(int i=0; i < team->size(); i++){
		Vector2 pos = v2_make(0, 0); 
		pos = action.move[i];
		Robot* robot = team->at(i);

		if( idClosest == robot->id() ){
#ifdef SKILL_OWNER
			cout << "SKILL DO ROBO MAIS PERTO DA BOLA: " << attacker->getCurrentState()->objectName().toStdString() << endl;
#endif
#ifdef DELTA_POS_OWNER
			static Vector2 lastPos = *pos;
			cout << v2_norm(v2_sub(lastPos, *pos)) << endl;
#endif
#ifdef MOVE_OWNER
			cout << pos.x << " " << pos.y << endl;
#endif

#ifdef KICK_POINT
			cout << action.kick_point.x << " " << action.kick_point.y << endl;
#endif
			attacker->setRobot(robot);
			attacker->updateSoccerAction(action.type, action.kick_point, pos);
			attacker->step();
			
#ifdef DELTA_POS_OWNER
			lastPos = *pos;
#endif
		}
		else{
			QLineF line = QLineF(robot->x(), robot->y(), ball->x(), ball->y());
			qreal orientation = line.angle() * PI / 180; //convenção sentido horario para classe QLineF

			//_max_skills.at(i)->setPoint(pos->x, pos->y);
			//_max_skills.at(i)->setSpeed(envReal.red_speed);
			//_max_skills.at(i)->setOrientation(orientation);
			//_max_skills.at(i)->step();

			//cout << pos->x << " " << pos->y << endl;
			//((GotoTactic*)player_[i])->setRobot(robot);
			Goto* g = ((GotoTactic*)player_[i])->goto_;
			//g->setAllowDefenseArea();
			g->setPoint(pos.x, pos.y);
			g->setSpeed(speed_);//envReal.red_speed);
			g->setOrientation(orientation);
			player_[i]->step();
		}
	}
}
