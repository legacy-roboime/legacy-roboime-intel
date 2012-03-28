#include "MachineTransition.h"
#include "State.h"

using namespace LibIntelligence;

MachineTransition::MachineTransition(QObject* parent, State* source, State* target, qreal probability) 
	: QObject(parent),
	probability_(probability),
	source_(source),
	target_(target)
{
}


MachineTransition::~MachineTransition(void)
{
}

qreal MachineTransition::probability()
{
	return probability_;
}

State* MachineTransition::getTarget()
{
	return target_;
}

State* MachineTransition::getSource()
{
	return source_;
}

