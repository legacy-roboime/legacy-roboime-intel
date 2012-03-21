#include "Component.h"

using namespace LibIntelligence;

Component::Component(const Component& c)
	: QObject(c.parent()), active(c.active), working(c.working) {}

Component::Component(QObject* p, bool a, bool w)
	: QObject(p),
	active(a),
	working(w) {}

Component& Component::operator=(const Component& o)
{
	setParent(o.parent());
	active = o.active;
	working = o.working;
	return *this;
}

bool Component::isWorking() const
{
	return working;
}

bool Component::isNotWorking() const
{
	return !working;
}

void Component::setWorking()
{
	if(!working) emit startedWorking();
	working = true;
}

void Component::setNotWorking()
{
	if(working) emit stoppedWorking();
	working = false;
}

bool Component::isActive() const
{
	return active;
}

bool Component::isNotActive() const
{
	return !active;
}

void Component::setActive()
{
	if(!active) emit reactivated();
	active = true;
}

void Component::setNotActive()
{
	if(active) emit deactivated();
	active = false;
}