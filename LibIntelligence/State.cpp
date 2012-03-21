#include "State.h"
#include "MachineTransition.h"

using namespace LibIntelligence;

State::State(QObject* parent) : 
	QObject(parent),
	outs_(QVector<MachineTransition*>())
{
}

State::State(const State& state) : QObject()//state)
{
	this->setObjectName(state.objectName());
}

State::~State(void)
{
	for(int i=0; i<outs_.size(); i++)
		delete outs_.at(i);
}

void State::pushTransition(MachineTransition* transition)
{
	outs_.push_back(transition);
}

int State::sizeTransitions()
{
	return outs_.size();
}

MachineTransition* State::getTransition(int pos)
{
	return outs_.at(pos);
}

bool State::busy()
{
	return false;
}