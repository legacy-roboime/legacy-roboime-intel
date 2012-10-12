/*
Command type 	Command Description 	Command
=================================================================================
Control commands
	Halt 	                        H
	Stop 	                        S
	Ready 	                        ' ' (space character)
	Start 	                        s
Game Notifications
	Begin first half 	        1
	Begin half time 	        h
	Begin second half 	        2
	Begin overtime half 1 	        o
	Begin overtime half 2 	        O
	Begin penalty shootout 	        a

	Command type 	Command Description 	Yellow Team Command 	Blue Team Command
	=================================================================================
	Game restarts
	Kick off 	                k                       K
	Penalty 	                p 	                P
	Direct Free kick 	        f 	                F
	Indirect Free kick 	        i 	                I
	Extras
	Timeout 	                t 	                T
	Timeout end 	                z 	                z
	Goal scored 	                g 	                G
	decrease Goal score 	        d 	                D
	Yellow Card 	                y 	                Y
	Red Card 	                r 	                R
	Cancel 	                        c
*/

#include "ObeyReferee.h"
#include "Ball.h"
#include "StopReferee.h"
#include "Halt.h"
#include "PenaltyThem.h"
#include "PenaltyUs.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Plays;

ObeyReferee::ObeyReferee(QObject *q, Play *p, Robot* gk, Robot* pKicker)
	: Play(q, p->team(), p->stage()),
	play(p),
	stopReferee(new StopReferee(this, p->team(), p->stage(), gk)),
	halt(new Halt(this, p->team(), p->stage())),
	penaltyUs(new PenaltyUs(this, p->team(), p->stage(), pKicker, gk)),
	penaltyThem(new PenaltyThem(this, p->team(), p->stage(), gk)),
	cmd('H'),
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

void ObeyReferee::setGoalkeeper(Robot* gk)
{
    ((StopReferee*)stopReferee)->setGoalkeeper(gk);
    ((PenaltyUs*)penaltyUs)->setGoalkeeper(gk);
    ((PenaltyThem*)penaltyThem)->setGoalkeeper(gk);
}

void ObeyReferee::step()
{
	Stage *sta = this->stage();
	qreal dist=-1;

	Ball *ball = sta->ball();

	dist = sqrt(pow(lastBall.x() - ball->x(), 2) + pow(lastBall.y() - ball->y(), 2));

	
	//Commands Referee
	if(cmd != sta->getCmdReferee()){
		lastCmd = cmd;
		cmd = sta->getCmdReferee();
		lastBall = *ball;
	}

#ifdef REFERRE_CMD
	cout << "JUIZ: " << cmd << endl;
#endif

	//if(lastCmd != cmd){
	////	lastCmd = cmd;
	//	lastBall = *ball;
	//}

	TeamColor usColor = team()->color();

	//Comportamento muda caso a bola tenha se deslocado
	if(cmd == 'f' && usColor == YELLOW)
		play->step();
	else if(cmd == 'f' && usColor == BLUE){
		if(dist<200 && ball->speed().length() < 200)
			stopReferee->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
	else if(cmd == 'F' && usColor == YELLOW){
		if(dist<200 && ball->speed().length() < 200)
			stopReferee->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
	else if(cmd == 'F' && usColor == BLUE)
		play->step();
	else if(cmd == 'i' && usColor == YELLOW)
		play->step();
	else if(cmd == 'i' && usColor == BLUE){
		if(dist<200 && ball->speed().length() < 200)
			stopReferee->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
	else if(cmd == 'I' && usColor == YELLOW){
		if(dist<200 && ball->speed().length() < 200)
			stopReferee->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
	else if(cmd == 'I' && usColor == BLUE)
		play->step();
	else if(cmd == 'k' || cmd == 'K')
		stopReferee->step();
    else if((cmd == 'p' && usColor == YELLOW) || (cmd == 'P' && usColor == BLUE))
		penaltyUs->step();
    else if((cmd == 'P' && usColor == YELLOW) || (cmd == 'p' && usColor == BLUE))
		penaltyThem->step();

	//Penalty, kickoff, normal start e force start (ordem dos else if importa)
	else if(cmd == ' ' && (lastCmd == 'k' || lastCmd == 'p') && usColor == YELLOW)
		play->step();
    else if(cmd == ' ' && lastCmd == 'k' && usColor == BLUE){
		if(dist<200 && ball->speed().length() < 200)
			stopReferee->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
    else if(cmd == ' ' && lastCmd == 'p' && usColor == BLUE){
		if(dist<200 && ball->speed().length() < 200)
			penaltyThem->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
    else if(cmd == ' ' && lastCmd == 'K' && usColor == YELLOW){
		if(dist<200 && ball->speed().length() < 200)
			stopReferee->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
    else if(cmd == ' ' && lastCmd == 'P' && usColor == YELLOW){
		if(dist<200 && ball->speed().length() < 200)
			penaltyThem->step();
		else{
			//lastBall = *ball;
			play->step();
		}
	}
    else if(cmd == ' ' && (lastCmd == 'K' || lastCmd == 'P') && usColor == BLUE)
		play->step();

	else if(cmd == ' ')
		play->step();
	else if(cmd == 's')
		play->step();

	//Comportamento halt, stop e jogo
	else if(cmd == 'H')
		halt->step();
	else if(cmd == 'S')
		stopReferee->step();
	else if(cmd == '1' || cmd == 'h' || cmd == '2' || cmd == 'o' || cmd == 'O' || cmd == 'a')
		stopReferee->step();
	else if(cmd == 't' || cmd == 'T')
		halt->step();
	else if(cmd == 'z' || cmd == 'g' || cmd == 'G' || cmd == 'd' || cmd == 'D' || cmd == 'y' || cmd == 'Y' || cmd == 'r' || cmd == 'R' || cmd == 'c')
		stopReferee->step();
	else
		play->step();
}
