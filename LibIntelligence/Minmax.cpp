#include "Minmax.h"
#include "Goal.h"
#include "Ball.h"
#include "Simulation.h"

#define LOGGING

using namespace LibIntelligence::Plays;
using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

Minmax::Minmax(QObject *parent, Team* team ,Stage* stage, int depth, int best_action):
Play(parent,team,stage),
	_depth(depth),
	_current_state(stage),
	_best_action(best_action),
	log("C:\\Users\\Bill\\Desktop\\log.dat"),
	steps_per_turn(10)
{
	_max_plays.push_back(new Plays::Halt(parent, team, stage));
	_max_plays.push_back(new Plays::StopReferee(parent, team, stage));
	_max_plays.push_back(new Plays::CBR2011(parent, team, stage));
	_min_plays.push_back(new Plays::CBR2011(parent, team, stage));
	log.open(QIODevice::ReadWrite);
}


Minmax::~Minmax()
{
	log.close();
	for(int i=0; i<_max_plays.size(); i++)
		delete _max_plays.at(i);
	for(int i=0; i<_min_plays.size(); i++)
		delete _min_plays.at(i);
}


void Minmax::step()
{
	//cout << "a " << stage()->blueTeam()->at(0)->x() << endl;
	getMaxValue( StageX(*stage()), _depth );
	//cout << "d " << stage()->blueTeam()->at(0)->x() << endl;
	//printf("%d ", _best_action);
	_max_plays[_best_action]->setStage( this->stage() );
	_max_plays[_best_action]->step();
}


qreal Minmax::getMaxValue(StageX s, int depth )
{
	qreal aux, value;

	value = -FLT_MAX;
	if( depth == 0 )
		return evaluate(&s);
	for( int i = 0; i < _max_plays.size(); i++ ){
		expandMax( &s, i );
		if( value < (aux = getMinValue( s, depth-1 )) ){
			value = aux;           
			_best_action = i;
		}
	} 
	return value;
}


qreal Minmax::getMinValue(StageX s, int depth )
{
	qreal aux, value;

	value = FLT_MAX;
	if( depth == 0 )
		return evaluate(&s);
	for( int i = 0; i < _min_plays.size(); i++ ){
		expandMin( &s, i );
		if( value > (aux = getMaxValue( s, depth-1 )) ){
			value = aux;           
		}
	} 
	return value;
}

qreal Minmax::evaluate( Stage* stage )
{
	// avaliacao do estado do jogo conmsiderando.
	// Valor alto bom para Max e ruim para Min.
	Team* team = this->team_;
	Ball* ball = stage->ball();
	Goal* enemyGoal;
	Goal* myGoal;

	if(team->color() == YELLOW){
		enemyGoal = stage->blueGoal();
		myGoal = stage->yellowGoal();
	}
	else{
		enemyGoal = stage->yellowGoal();
		myGoal = stage->blueGoal();
	}

	qreal dx1 = enemyGoal->x() - stage->ball()->x();
	qreal dy1 = enemyGoal->y() - stage->ball()->y();

	qreal dx2 = myGoal->x() - stage->ball()->x();
	qreal dy2 = myGoal->y() - stage->ball()->y();

	qreal dist1 = sqrt(dx1*dx1 + dy1*dy1);
	qreal dist2 = sqrt(dx2*dx2 + dy2*dy2);

	//printf("%f\n", dist2 - dist1);
	return (dist2 - dist1);
}



void Minmax::expandMin( StageX *s, int action )
{
	_min_plays[action]->setStage(s);
	_min_plays[action]->step();
	//cout << "a " << s->blueTeam()->at(0)->x() << endl;
	for(int i=0; i<steps_per_turn; i++)
		s->simulate(0.1);
	//cout << "d " << s->blueTeam()->at(0)->x() << endl;

#ifdef LOGGING
	Ball* ball = s->ball();
	Team* yellowTeam = s->yellowTeam();
	Team* blueTeam = s->blueTeam();
	QString str("3 0 ");
	for(int i=0; i<5; i++){
		Robot* robot = yellowTeam->at(i);
		str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speedX()) + " " + QString::number(robot->speedY()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
	}
	for(int i=0; i<5; i++){
		Robot* robot = blueTeam->at(i);
		str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speedX()) + " " + QString::number(robot->speedY()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
	}
	str += QString::number(ball->x()) + " " + QString::number(ball->y()) + " " + QString::number(ball->speedX()) + " " + QString::number(ball->speedY()) + " " + QString::number(0) + "\n";

	QDataStream out(&log);   // write the data
	out << str;
	log.flush();
#endif
}


void Minmax::expandMax( StageX *s, int action )
{
	_max_plays[action]->setStage(s);
	_max_plays[action]->step();
}
