#include "MachineTransition.h"
#include "State.h"

using namespace LibIntelligence;

MachineTransition::MachineTransition(QObject* parent, QObject* metaObj, const char* method, State* source, State* target, qreal probability) 
	: QObject(parent),
	metaObj_(metaObj),
	method_(method),
	probability_(probability),
	source_(source),
	target_(target)
{
}


MachineTransition::~MachineTransition(void)
{
}

bool MachineTransition::condition()
{
	bool cond;
	QMetaObject::invokeMethod(metaObj_, method_, Qt::DirectConnection, Q_RETURN_ARG(bool, cond));
	return cond;
}

qreal MachineTransition::probability()
{
	return probability_;
}

State* MachineTransition::getTarget()
{
	return target_;
}

