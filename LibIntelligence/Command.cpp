#include "Command.h"
#include "messages_internal_command.pb.h"

namespace LibIntelligence
{
	struct CommandImpl
	{
	public:
		CommandImpl(CommandImpl* c)
			: timestamp(c->timestamp),
			id(c->id),
			kickSpeed(c->kickSpeed),
			dribbleSpeed(c->dribbleSpeed),
			wheelSpeeds(c->wheelSpeeds),
			old(c->old),
			forcedDribble(c->forcedDribble),
			forcedKick(c->forcedKick) {}

		CommandImpl(quint8 id, qreal ks, qreal ds, const QVector<qreal>& ws)
			: timestamp(0.0),
			id(id),
			kickSpeed(ks),
			dribbleSpeed(ds),
			wheelSpeeds(ws),
			old(false),
			forcedDribble(false),
			forcedKick(false) {}

		double timestamp;//TODO: implement timestamping
		//CommandRobot* command;
		quint8 id;
		qreal kickSpeed;
		qreal dribbleSpeed;
		QVector<qreal> wheelSpeeds;
		bool old, forcedKick, forcedDribble;
	};
}

using namespace LibIntelligence;

Command::Command(const Command& c)
	: pimpl(new CommandImpl(c.pimpl)){}

//Command::Command(QObject* parent, int i, const QVector<qreal>& speed, qreal d, qreal k) : QObject(parent),
Command::Command(quint8 i, const QVector<qreal>& s, qreal d, qreal k)
	: pimpl(new CommandImpl(i, k, d, s))//command(new CommandRobot())
	//_id(i),
	//_force_kick(false),
	//_force_dribble(false),s
	//_old(false),
	//_wheel_speed(speed),
	//_kick_speed(k),
	//_dribble_speed(d) {}
{
	//command->set_id(i);
	//command->mutable_dribbler()->set_speed(d);
	//command->mutable_kicker()->set_speed(k);
	//for(int i = 0; i < s.size(); i++) {
	//	CommandWheel* cw = command->add_wheels();
	//	cw->set_id(i);
	//	cw->set_speed(s[i]);
	//}
}

Command::~Command()
{
	delete pimpl;
}

void Command::setId(quint8 i)
{
	pimpl->id = i;
	//command->set_id(i);
	//_id = i;
}

int Command::id() const
{
	//return _id;
	//return command->id();
	return pimpl->id;
}

void Command::setKickSpeed(qreal s)
{
	pimpl->kickSpeed = s;
	//command->mutable_kicker()->set_speed(s);
	//_kick_speed = speed;
}

qreal Command::kickSpeed() const
{
	//return _kick_speed;
	//return command->mutable_kicker()->speed();
	return pimpl->kickSpeed;
}

void Command::setDribbleSpeed(qreal s)
{
	pimpl->dribbleSpeed = s;
	//command->mutable_dribbler()->set_speed(s);
	//_dribble_speed = speed;
}

qreal Command::dribbleSpeed() const
{
	//return _dribble_speed;
	//return command->mutable_dribbler()->speed();
	return pimpl->dribbleSpeed;
}

void Command::setWheelsSpeed(const QVector<qreal>& s)
{
	//command->clear_wheels();
	//for(int i = 0; i < s.size(); i++) {
	//	CommandWheel* cw = command->add_wheels();
	//	cw->set_id(i);
	//	cw->set_speed(s[i]);
	//}
	pimpl->wheelSpeeds.clear();
	pimpl->wheelSpeeds << s;
	//pimpl->wheelSpeeds = s;
}

const QVector<qreal>& Command::wheelsSpeed() const
{
	//static QVector<qreal> ws;
	//ws.clear();
	//ws.resize(command->wheels_size());
	//for(int i = 0; i < ws.size(); i++) {
	//	ws[i] = command->wheels(i).speed();
	//}
	//return ws;
	return pimpl->wheelSpeeds;
}

qreal Command::wheelSpeedAt(quint32 i) const
{
	//return i < pimpl->wheelSpeeds.size() ? pimpl->wheelSpeeds[i] : 0.0;
	return pimpl->wheelSpeeds.at(i);
}

quint32 Command::wheelsSize() const
{
	//return command->wheels().size();
	return pimpl->wheelSpeeds.size();
}

void Command::setTimestamp(double t)
{
	//timestamp_ = t;
	pimpl->timestamp = t;
}

void Command::setTimestampNow()
{
	//TODO: implement timestamping here
}

double Command::timestamp() const
{
	//return timestamp_;
	return pimpl->timestamp;
}

void Command::setForcedKick(qreal speed)
{
	setKickSpeed(speed);
	pimpl->forcedKick = true;
	//command->mutable_kicker()->set_forced(true);
	//_force_kick = true;
}

bool Command::isForcedKick() const
{
	//return _force_kick;
	//return command->mutable_kicker()->forced();
	return pimpl->forcedKick;
}

void Command::setForcedDribble(qreal speed)
{
	setDribbleSpeed(speed);
	pimpl->forcedDribble = true;
	//command->mutable_dribbler()->set_forced(true);
	//_force_dribble = true;
}

bool Command::isForcedDribble() const
{
	//return _force_dribble;
	//return command->mutable_dribbler()->forced();
	return pimpl->forcedDribble;
}

void Command::setOld()
{
	//old_ = true;
	pimpl->old = true;
}

bool Command::isOld() const
{
	//return old_;
	return pimpl->old;
}

void Command::clear()
{
	//TODO: do
	//command->Clear();
	for(int i=0; i<pimpl->wheelSpeeds.size(); i++) pimpl->wheelSpeeds[i] = 0.0;
	pimpl->kickSpeed = 0.0;
	pimpl->dribbleSpeed = 0.0;
	//_id = -1;//do nothing here
	pimpl->old = false;
	pimpl->forcedKick = false;
	pimpl->forcedDribble = false;
}

const CommandRobot& Command::raw() const
{
	return *(new CommandRobot());
}