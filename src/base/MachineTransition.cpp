#include "MachineTransition.h"
#include "State.h"

using namespace LibIntelligence;

MachineTransition::MachineTransition(QObject* parent, State* source, State* target, qreal probability) 
	: QObject(parent),
	source_(source),
	target_(target),
	probability_(probability)
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

