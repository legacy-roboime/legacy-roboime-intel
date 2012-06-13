#ifdef USE_PHYSX
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
	steps_per_turn(18)
{
	log.open(QIODevice::WriteOnly);

	LinearCombPlay* lin_comb_play0 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play1 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play2 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play3 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play4 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play5 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play6 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play7 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play8 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play9 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play10 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play11 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play12 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play13 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play14 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play15 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play16 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play17 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play18 = new Plays::LinearCombPlay(this, team, stage);
	LinearCombPlay* lin_comb_play19 = new Plays::LinearCombPlay(this, team, stage);	

	qreal c1[] = {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0};
	qreal c2[] = {0,0,.5,.5,0,0,0,0,0,0,0,0,0,0,0};
	qreal c3[] = {0,0,.5,0,.5,0,0,0,0,0,0,0,0,0,0};
	qreal c4[] = {0,0,.5,0,0,.5,0,0,0,0,0,0,0,0,0};
	qreal c5[] = {0,0,.5,0,0,0,.5,0,0,0,0,0,0,0,0};
	qreal c6[] = {0,0,.5,0,0,0,0,.5,0,0,0,0,0,0,0};

	qreal c7[] =  {0,0,1.2,-.2,0,0,0,0,0,0,0,0,0,0,0};
	qreal c8[] =  {0,0,1.2,0,-.2,0,0,0,0,0,0,0,0,0,0};
	qreal c9[] =  {0,0,1.2,0,0,-.2,0,0,0,0,0,0,0,0,0};
	qreal c10[] = {0,0,1.2,0,0,0,-.2,0,0,0,0,0,0,0,0};
	qreal c11[] = {0,0,1.2,0,0,0,0,-.2,0,0,0,0,0,0,0};

	qreal c12[] = {0,-.2,1.2,0,0,0,0,0,0,0,0,0,0,0};
	qreal c13[] = {.2,0,.8,0,0,0,0,0,0,0,0,0,0,0,0};

	qreal c14[] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	qreal c15[] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


	qreal c16[] = {0,0,0,.5,.5,0,0,0,0,0,0,0,0,0,0};
	qreal c17[] = {0,0,0,.5,0,.5,0,0,0,0,0,0,0,0,0};
	qreal c18[] = {0,0,0,.5,0,0,.5,0,0,0,0,0,0,0,0};
	qreal c19[] = {0,0,0,.5,0,0,0,.5,0,0,0,0,0,0,0};
	qreal c20[] = {0,0,0,0,.5,.5,0,0,0,0,0,0,0,0,0};
	qreal c21[] = {0,0,0,0,.5,0,.5,0,0,0,0,0,0,0,0};
	qreal c22[] = {0,0,0,0,.5,0,0,.5,0,0,0,0,0,0,0};
	qreal c23[] = {0,0,0,0,0,.5,.5,0,0,0,0,0,0,0,0};
	qreal c24[] = {0,0,0,0,0,.5,0,.5,0,0,0,0,0,0,0};
	qreal c25[] = {0,0,0,0,0,0,.5,.5,0,0,0,0,0,0,0};

	qreal c26[] = {.7,0,.3,0,0,0,0,0,0,0,0,0,0,0,0};

	qreal c27[] = {0,-.2,1.2,0,0,0,0,0,0,0,0,0,0,0,0};

	qreal c28[] = {0,0,-.3,1.3,0,0,0,0,0,0,0,0,0,0,0};


	// Movimentos
	qreal c29[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
	qreal c30[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0};
	qreal c31[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	qreal c32[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1};

	qreal c33[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1};
	qreal c34[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,-1,1};
	qreal c35[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,-1,1};
	qreal c36[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1};

	// Entre a Bola e o Gol
	qreal c37[] = {0,.6,.4,0,0,0,0,0,0,0,0,0,0,0,0};
	//cout << "sdsdasddas\n";

	/*	lin_comb_play0->setCoefs( c37,c36, c37, c26, c26 );
	lin_comb_play1->setCoefs( c37,c29,  c37, c26, c26 );
	lin_comb_play2->setCoefs( c37,c30,  c37, c26, c26 );
	lin_comb_play3->setCoefs( c37,c31, c37, c26, c26 );
	lin_comb_play4->setCoefs( c37,c32, c37, c26, c26 );
	lin_comb_play5->setCoefs( c37,c33, c37, c26, c26 );
	lin_comb_play6->setCoefs( c37,c34,  c37, c26, c26 );
	lin_comb_play7->setCoefs( c37,c35, c37, c26, c26 );
	//	lin_comb_play8->setCoefs( c37,c36, c37, c37, c26, c26 ); */

	lin_comb_play1->setCoefs(c6, c6, c37, c1, c1 );
	lin_comb_play2->setCoefs(c6, c6, c26, c26, c26 ); 
	lin_comb_play3->setCoefs(c27, c15, c1, c26, c26 );
	lin_comb_play4->setCoefs(c27, c27, c6, c26, c26 );
	lin_comb_play5->setCoefs(c15, c27, c37, c26, c26 );
	lin_comb_play6->setCoefs(c37, c27, c37, c26, c26 );
	lin_comb_play7->setCoefs(c1, c37, c37, c26, c26 ); 
	lin_comb_play8->setCoefs(c29, c37, c37, c26, c26 );
	lin_comb_play9->setCoefs(c30, c37, c37, c26, c26 );
	lin_comb_play10->setCoefs(c31, c37, c37, c26, c26 );
	lin_comb_play11->setCoefs(c32, c37, c37, c26, c26 );
	lin_comb_play12->setCoefs(c33, c37, c37, c26, c26 );
	lin_comb_play13->setCoefs(c34, c37, c37, c26, c26 );
	lin_comb_play14->setCoefs(c35, c37, c37, c26, c26 );
	lin_comb_play15->setCoefs(c36, c37, c37, c26, c26 );

	//	lin_comb_play7->setCoefs(c17, c23, c9, c10, c19 );
	//	lin_comb_play8->setCoefs(c8, c9, c1, c22, c12 );
	//	lin_comb_play9->setCoefs(c9, c4, c14, c12, c13 );
	//	lin_comb_play10->setCoefs(c2, c19, c12, c21, c13 );
	/*	lin_comb_play1->setCoefs(c1, c2, c3, c4, c5 );
	lin_comb_play2->setCoefs(c12, c5, c14, c3, c2 ); 
	lin_comb_play3->setCoefs(c4, c15, c23, c7, c7 );
	lin_comb_play4->setCoefs(c12, c5, c1, c12, c8 );
	lin_comb_play5->setCoefs(c15, c9, c7, c24, c9 );
	lin_comb_play6->setCoefs(c6, c2, c10, c1, c10 );
	lin_comb_play7->setCoefs(c17, c23, c9, c10, c19 );
	lin_comb_play8->setCoefs(c8, c9, c1, c22, c12 );
	lin_comb_play9->setCoefs(c9, c4, c14, c12, c13 );
	lin_comb_play10->setCoefs(c2, c19, c12, c21, c13 );
	lin_comb_play11->setCoefs(c11, c12, c9, c6, c2 );
	lin_comb_play12->setCoefs(c4, c25, c15, c2, c3 );
	lin_comb_play13->setCoefs(c13, c1, c19, c3, c4 );
	lin_comb_play14->setCoefs(c14, c14, c24, c14, c14 );
	lin_comb_play15->setCoefs(c15, c15, c15, c15, c15 );
	lin_comb_play16->setCoefs(c1, c1, c1, c1, c1 );
	/*
	lin_comb_play1->setCoefs(c1, c2, c3, c4, c5 );
	lin_comb_play2->setCoefs(c2, c3, c4, c5, c6 ); 
	lin_comb_play3->setCoefs(c3, c4, c5, c6, c7 );
	lin_comb_play4->setCoefs(c4, c5, c6, c7, c8 );
	lin_comb_play5->setCoefs(c5, c6, c7, c8, c9 );
	lin_comb_play6->setCoefs(c6, c7, c8, c9, c10 );
	lin_comb_play7->setCoefs(c7, c8, c9, c10, c11 );
	lin_comb_play8->setCoefs(c8, c9, c10, c11, c12 );
	lin_comb_play9->setCoefs(c9, c10, c11, c12, c13 );
	lin_comb_play10->setCoefs(c10, c11, c12, c13, c1 );
	lin_comb_play11->setCoefs(c11, c12, c13, c1, c2 );
	lin_comb_play12->setCoefs(c12, c13, c1, c2, c3 );
	lin_comb_play13->setCoefs(c13, c1, c2, c3, c4 );
	lin_comb_play14->setCoefs(c14, c14, c14, c14, c14 );
	lin_comb_play15->setCoefs(c15, c15, c15, c15, c15 );
	lin_comb_play16->setCoefs(c1, c1, c1, c1, c1 );*/

	//	_max_plays.push_back(new Plays::AutoAttacker2(parent, team, stage));
	//	_max_plays.push_back(new Plays::AutoAttacker3(parent, team, stage));
	//	_max_plays.push_back(new Plays::AutoAttacker(parent, team, stage));
	//	_max_plays.push_back(new Plays::CBR2011(parent, team, stage));ana
	//	_max_plays.push_back(lin_comb_play0);
	_max_plays.push_back(lin_comb_play1);
	_max_plays.push_back(lin_comb_play2);
	_max_plays.push_back(lin_comb_play3);
	_max_plays.push_back(lin_comb_play4);
	_max_plays.push_back(lin_comb_play5);
	_max_plays.push_back(lin_comb_play6);
	_max_plays.push_back(lin_comb_play7);
	_max_plays.push_back(lin_comb_play8);
	_max_plays.push_back(lin_comb_play9);
	_max_plays.push_back(lin_comb_play10);
	_max_plays.push_back(lin_comb_play11);
	_max_plays.push_back(lin_comb_play12);
	_max_plays.push_back(lin_comb_play13);
	_max_plays.push_back(lin_comb_play14);
	_max_plays.push_back(lin_comb_play15);
	//	_max_plays.push_back(lin_comb_play16);
	/*	_max_plays.push_back(lin_comb_play17);
	_max_plays.push_back(lin_comb_play18);
	_max_plays.push_back(lin_comb_play19); */

	//	_max_plays.push_back(new Plays::CBR2011(parent, team, stage));
	_min_plays.push_back(new Plays::CBR2011(parent, team, stage));
}


Minmax::~Minmax()
{
	for(int i=0; i<_max_plays.size(); i++)
		delete _max_plays.at(i);
	for(int i=0; i<_min_plays.size(); i++)
		delete _min_plays.at(i);
	log.close();
}


void Minmax::step()
{
	//cout << "a " << stage()->blueTeam()->at(0)->x() << endl;
	getMaxValue( StageX(*stage()), _depth );
	//cout << "d " << stage()->blueTeam()->at(0)->x() << endl;
	printf("%d ", _best_action);
	_max_plays[_best_action]->setStage( this->stage() );
	//_max_plays[_best_action]->setTeam( this->stage()->getTeamFromColor(this->team()->color()) );//(Team*) this->team() ) ;
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
	Team* team = stage->getTeamFromColor(this->team()->color());//this->team_;
	Ball* ball = stage->ball();
	Goal* enemyGoal;
	Goal* myGoal;
	Object* enemy_closest_to_ball = team->enemyTeam()->getClosestPlayerToBall();
	Object* player_closest_to_ball = team->getClosestPlayerToBall(); 

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

	qreal dx3 = stage->ball()->x() - player_closest_to_ball->x() ;
	qreal dy3 = stage->ball()->y() - player_closest_to_ball->y();

	qreal dx4 = enemy_closest_to_ball ->x() - stage->ball()->x();
	qreal dy4 = enemy_closest_to_ball ->y() - stage->ball()->y();

	qreal dx5 =  enemyGoal->x() - player_closest_to_ball->x();
	qreal dy5 =  enemyGoal->y() - player_closest_to_ball->y();



	qreal dist1 = sqrt(dx1*dx1 + dy1*dy1);
	qreal dist2 = sqrt(dx2*dx2 + dy2*dy2);
	qreal dist3 = sqrt(dx3*dx3 + dy3*dy3);
	qreal dist4 = sqrt(dx4*dx4 + dy4*dy4);
	qreal dist5 = sqrt(dx5*dx5 + dy5*dy5);

	qreal attack_cos_angle = (dx3*dx5 + dy3*dy5)/(dist3*dist5);

	//printf("d= %f, cos_angle = %f\n",dist3, attack_cos_angle);
	//return (dist2 - dist1);
	return  ( dist2<300? 100000*dist2: 0)  -dist1 + (dist3<1000? 50*attack_cos_angle :0)+ /*(dist1>500? .1*dist4: 0 )*/ +(dist1>300?  -.8*dist3: 0 ) ;// //-dist3 -20*dist1; /*+ (dist3<50? 20*attack_cos_angle :0);// -10*dist1 ;*/
}



void Minmax::expandMin( StageX *s, int action )
{
	_min_plays[action]->setStage(s);
	//_min_plays[action]->setTeam(s->getTeamFromColor(this->team()->color()));
	_min_plays[action]->step();

	for(int i=0; i<steps_per_turn; i++){
		s->simulate(0.1);

#ifdef LOGGING
		QDataStream out(&log);   // write the data
		Ball* ball = s->ball();
		Team* yellowTeam = s->yellowTeam();
		Team* blueTeam = s->blueTeam();
		QString str("3 0 ");
		for(int i=0; i<5; i++){
			Robot* robot = yellowTeam->at(i);
			str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speed().x()) + " " + QString::number(robot->speed().y()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
		}
		//cout << str.toStdString() << endl;
		for(int i=0; i<5; i++){
			Robot* robot = blueTeam->at(i);
			str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speed().x()) + " " + QString::number(robot->speed().y()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
		}
		str += QString::number(ball->x()) + " " + QString::number(ball->y()) + " " + QString::number(ball->speed().x()) + " " + QString::number(ball->speed().y()) + " " + QString::number(0) + "\n";
		out << str;
		log.flush();
#endif
	}
}


void Minmax::expandMax( StageX *s, int action )
{
	_max_plays[action]->setStage(s);
	//_max_plays[action]->setTeam(s->getTeamFromColor(this->team()->color()));
	_max_plays[action]->step();
}

#endif

