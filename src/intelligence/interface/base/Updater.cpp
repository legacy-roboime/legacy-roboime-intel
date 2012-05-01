#include "Updater.h"
#include "Update.h"

using namespace LibIntelligence;

Updater::Updater(QObject* parent)
	: QObject(parent) {}

Updater::~Updater() {}

void Updater::step()
{
	receive();
	prepare();
	//updates_.apply(this);//hmm, naaah
}

void Updater::apply()
{
	updates_.apply(this);
}

void Updater::add(Robot* r)
{
	robots.append(r);
}

void Updater::del(Robot* r)
{
	robots.removeAll(r);
}

quint32 Updater::robotsSize() const
{
	return robots.size();
}

Robot* Updater::robot(quint32 i) const
{
	return robots[i];
}

void Updater::add(Ball* b)
{
	balls.append(b);
}

void Updater::del(Ball* b)
{
	balls.removeAll(b);
}

quint32 Updater::ballsSize() const
{
	return balls.size();
}

Ball* Updater::ball(quint32 i) const
{
	return balls[i];
}

void Updater::add(Stage* s)
{
	stages.append(s);
}

void Updater::del(Stage* s)
{
	stages.removeAll(s);
}

quint32 Updater::stagesSize() const
{
	return stages.size();
}

Stage* Updater::stage(quint32 i) const
{
	return stages[i];
}

void Updater::prepare() {}

void Updater::receive() {}

Updates*const Updater::updates()
{
	return &updates_;
}

void Updater::enqueue(Update* u)
{
	updates_.enqueue(u);
	emit receivedUpdate();
}

Update* Updater::dequeue()
{
	return updates_.dequeue();
}