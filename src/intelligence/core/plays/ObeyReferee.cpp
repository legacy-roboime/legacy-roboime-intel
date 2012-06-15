#include "ObeyReferee.h"
#include "Ball.h"
#include "StopReferee.h"
#include "Halt.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Plays;

ObeyReferee::ObeyReferee(QObject *q, Play *p)
	: Play(q, p->team(), p->stage()),
	play(p),
	halt(new Halt(this, p->team(), p->stage())),
	stopReferee(new StopReferee(this, p->team(), p->stage())),
	lastCmd('H')
{}

ObeyReferee::~ObeyReferee()
{
	delete play;
	delete halt;
	delete stopReferee;
}

void ObeyReferee::setPlay(Play *p)
{
	if(p != play) {
		delete play;
		play = p;
	}
}

void ObeyReferee::setTeam(Team *t)
{
	if(t != team()) {
		Play::setTeam(t);
		play->setTeam(t);
		halt->setTeam(t);
		stopReferee->setTeam(t);
	}
}

void ObeyReferee::setStage(Stage *s)
{
	if(s != stage()) {
		Play::setStage(s);
		play->setStage(s);
		halt->setStage(s);
		stopReferee->setStage(s);
	}
}

void ObeyReferee::step()
{
	Stage *sta = this->stage();
	//Commands Referee
	char cmd = sta->getCmdReferee();//sta->getCmdReferee(); //TODO: utilizar o getCmdReferee

#ifdef REFERRE_CMD
	cout << "JUIZ: " << cmd << endl;
#endif

	lastCmd = 'H';
	qreal dist=-1;

	Ball *ball = sta->ball();

	dist = sqrt(pow(lastBall.x() - ball->x(), 2) + pow(lastBall.y() - ball->y(), 2));

	if(lastCmd != cmd){
		lastCmd = cmd;
		lastBall = *ball;
	}

	TeamColor usColor = team()->color();

	if(cmd == 'f' && usColor == TeamColor::YELLOW)
		play->step();
	else if(cmd == 'f' && usColor == TeamColor::BLUE){
		if(dist<100)
			stopReferee->step();
		else
			play->step();
	}
	else if(cmd == 'F' && usColor == TeamColor::YELLOW){
		if(dist<100)
			stopReferee->step();
		else
			play->step();
	}
	else if(cmd == 'F' && usColor == TeamColor::BLUE)
		play->step();

	else if(cmd == 'i' && usColor == TeamColor::YELLOW)
		play->step();
	else if(cmd == 'i' && usColor == TeamColor::BLUE){
		if(dist<100)
			stopReferee->step();
		else
			play->step();
	}
	else if(cmd == 'I' && usColor == TeamColor::YELLOW){
		if(dist<100)
			stopReferee->step();
		else
			play->step();
	}
	else if(cmd == 'I' && usColor == TeamColor::BLUE)
		play->step();

	else if(cmd == 'H')
		halt->step();
	else if(cmd == 'S')
		stopReferee->step();
	else if(cmd == ' ')
		play->step();
	else if(cmd == 's')
		play->step();

	else if(cmd == '1' || cmd == 'h' || cmd == '2' || cmd == 'o' || cmd == 'O' || cmd == 'a')
		stopReferee->step();

	else if(cmd == 'k' || cmd == 'K')
		stopReferee->step();

	else if(cmd == 't' || cmd == 'T')
		halt->step();

	else if(cmd == 'z' || cmd == 'g' || cmd == 'G' || cmd == 'd' || cmd == 'D' || cmd == 'y' || cmd == 'Y' || cmd == 'r' || cmd == 'R' || cmd == 'c')
		stopReferee->step();

	else if(cmd == 'p' && usColor == TeamColor::YELLOW)
		stopReferee->step();
	else if(cmd == 'p' && usColor == TeamColor::BLUE)
		stopReferee->step();
	else if(cmd == 'P' && usColor == TeamColor::YELLOW)
		stopReferee->step();
	else if(cmd == 'P' && usColor == TeamColor::BLUE)
		stopReferee->step();

	else
		play->step();
}
