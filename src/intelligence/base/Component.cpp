#include "Component.h"

using namespace LibIntelligence;

Component::Component(const Component& c)
	: QObject(c.parent()), active(c.active), working(c.working) {}

Component::Component(QObject* p, bool a, qreal w)
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

qreal Component::isWorking() const
{
	return working;
}

void Component::setWorking(qreal w)
{
	if(working<=0) emit startedWorking(w);
	working = w;
}

void Component::setNotWorking()
{
	if(working>0) emit stoppedWorking();
	working = 0;
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