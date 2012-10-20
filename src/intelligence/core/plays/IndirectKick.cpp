#include "IndirectKick.h"
#include "Ball.h"
#include "Stage.h"
#include "Goal.h"
#include "Tactics.h"
#include "Skills.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

IndirectKick::IndirectKick(QObject *parent, Team* team, Stage* stage, Robot* gk, qreal speed)
	: StopReferee(parent, team, stage, gk),
	stateMachine(START),
	passedGoto(new Goto(this, team->at(0), Point(0, 0), team->enemyGoal(), speed)),
	sk(new SampledKick(this, team->at(0), NULL, true, 0, 1, speed, true)),
	attack(new Zickler43(this, team->at(0), speed))
{
}

void IndirectKick::restart(){
	stateMachine.setState(START);
}

bool IndirectKick::finished(){
	return stateMachine.currentState()==END;
}

void IndirectKick::step()
{
	qreal distPasserBall=0;

	StopReferee::step();
	stateMachine.step();


	Team* team = this->team_;
	Goal* myGoal = team->goal();
	Stage* stage = this->stage_;
	Ball* ball = stage->ball();
	map<qreal, Robot*> nearTeam = stage->getClosestPlayersToBallThatCanKick(team); //Nossos robos por ordem de proximidade da bola (exceto o goleiro)

	Object pos(*(stage->getGoalFromOtherColor(team->color())));
	pos+=Point((pos.x()<0)?1000:-1000,0);
	switch(stateMachine.currentState()){
	case START:
		passedRobot=NULL;
		passerRobot=NULL;
		//TODO refinar a seleção do passador e do recebedor
		if(nearTeam.size()>=2){
			map<qreal, Robot*>::iterator it1 = nearTeam.begin();
			passerRobot=(*it1).second;
			it1++;
			passedRobot=(*it1).second;
		}
		if(passedRobot){
			passedGoto->setRobot(passedRobot);
			stateMachine.setState(GO_POSITION);
		} else {
			stateMachine.setState(END);
		}
		break;
	case GO_POSITION:
		if(Line(*passedRobot,Point(pos)).length()<200){
			stateMachine.setState(PASS);
			break;
		}
		passedGoto->setPoint(pos);
		passedGoto->step();
		break;
	case PASS:
		if(passerRobot->touchedBall()){
			stateMachine.setState(TOUCHED);
			break;
		}
		sk->setRobot(passerRobot);
		sk->setRefLookPoint(passedRobot);
		sk->step();
		passedGoto->setPoint(&pos);
		passedGoto->step();
		break;
	case TOUCHED:
		if(!passerRobot->isLastToucher()){
			stateMachine.setState(END);
			break;
		}//no else
		{
			map<qreal, Robot*>::iterator it1 = nearTeam.begin();
			Robot* passerAlternative;
			passerAlternative=(*it1).second;
			if(passerAlternative->isLastToucher()){
				it1++;
				passerAlternative=(*it1).second;
			}
			attack->setRobot(passerAlternative);
			attack->step();
		}
		break;
	case END:
		break;
	default:
		break;
	}
}


PlayStateMachine::PlayStateMachine(unsigned int newState){
	oldState=newState;
	currentState_=newState;
	lastState=newState;
	timeEntryState=QTime::currentTime();
}

void PlayStateMachine::step()
{
	if(oldState!=currentState_){
		lastState=oldState;
		oldState=currentState_;
		timeEntryState=QTime::currentTime();
		timeEntryState.start();
	}
	timeInState=timeEntryState.elapsed();
}


void PlayStateMachine::setState(unsigned int newState){
	currentState_=newState;
}