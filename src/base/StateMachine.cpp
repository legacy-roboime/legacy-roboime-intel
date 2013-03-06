#include "StateMachine.h"
#include "Sampler.h"
#include "MachineTransition.h"
#include <QList>

using namespace LibIntelligence;

StateMachine::StateMachine(QObject* parent, bool deterministic) 
	: QObject(parent),
	states_(QVector<State*>()),
	deterministic_(deterministic)
{
}

StateMachine::StateMachine(QObject* parent, const StateMachine& machine) : QObject(parent)//machine)
{
	deterministic_ = machine.deterministic_;
	initial_ = getStateByName(machine.initial_->objectName());
	final_ = getStateByName(machine.final_->objectName());
	current_ = getStateByName(machine.current_->objectName());
	//states_ = QVector<State*>(machine.states_);
}


StateMachine::~StateMachine(void)
{
	//for(int i=0; i<states_.size(); i++)
	//	delete states_.at(i);
}

void StateMachine::execute()
{
	//if(!current_->busy()){
		QList<MachineTransition*> transitions = QList<MachineTransition*>();
		for(int i=0; i<current_->sizeTransitions(); i++){
			MachineTransition* transition = current_->getTransition(i);
			if(transition->condition())
				transitions.push_back(transition);
		}
		if(deterministic_){
			if(!transitions.isEmpty())
				current_ = transitions.at(0)->getTarget(); //colocar a transicao deterministica na posicao 0 caso haja mais de uma condicao satisfeita
		}
		else{
			qreal rndVal = Sampler::randFloat();
			qreal tempSum = 0;
			qreal probSum = 0;
			for(int i=0; i<transitions.size(); i++)
				probSum += transitions.at(i)->probability();
			rndVal *= probSum;
			for(int i=0; i<transitions.size(); i++){
				MachineTransition* transition = transitions.at(i);
				tempSum += transition->probability();
				if(tempSum > rndVal){
					current_ = transition->getTarget();
					break;
				}
			}
		}
	//}
}

State* StateMachine::getStateByName(const QString& name)
{
	for(int i=0; i<states_.size(); i++){
		if(states_.at(i)->objectName() == name)
			return states_.at(i);
	}
	return NULL;
}

State* StateMachine::getCurrentState()
{
	return current_;
}

State* StateMachine::getFinalState()
{
	return final_;
}

State* StateMachine::getInitialState()
{
	return initial_;
}

bool StateMachine::getDeterministic()
{
	return deterministic_;
}

QVector<State*>* StateMachine::getStates()
{
	return &states_;
}

void StateMachine::setCurrentState(State* current)
{
	current_ = current;
}

void StateMachine::pushState(State* state)
{
	states_.push_back(state);
}

void StateMachine::pushTransition(State* source, MachineTransition* transition)
{
	source->pushTransition(transition);
}

void StateMachine::setInitialState(State* initial)
{
	initial_ = initial;
}

void StateMachine::setFinalState(State* final)
{
	final_ = final;
}

void StateMachine::reset()
{
	current_ = initial_;
}

bool StateMachine::busy()
{
	return current_->busy();
}

