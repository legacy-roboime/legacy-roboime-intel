#include "Commander.h"

using namespace LibIntelligence;

Commander::Commander() {}

Commander::~Commander() {}

void Commander::add(Robot* r)
{
	robots.push_back(r);
}

void Commander::del(Robot* r)
{
	robots.removeAll(r);
}

void Commander::prepare() {}

void Commander::send() {}

void Commander::step()
{
	prepare();
	send();
}