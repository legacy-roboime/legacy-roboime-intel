#include "CommanderTx.h"
#include "Command.h"
#include "Robot.h"
#include <sstream>
#include <QtNetwork>
#include <QHostAddress>
#include <QVector>
#include "messages_internal_command.pb.h"

using namespace std;
using namespace LibIntelligence;

CommanderTx::CommanderTx(QObject* parent, char* address, quint16 port)
	: UdpServer(parent, address, port),
	Commander()
{
	connect(parent, SIGNAL(readySend()), this, SLOT(send()));
}

CommanderTx::~CommanderTx()
{
}

void CommanderTx::step()
{
	stringstream out;
	ToTransmission tot;
	for(int i = 0; i < robots.size(); i++) {
		CommandRobot* rob = tot.add_robots();
		//rob->MergeFrom(robots[n]->command().raw());
		//GAMBIARRA:
		rob->set_id(robots[i]->id());
		rob->mutable_kicker()->set_speed(robots[i]->command().kickSpeed());
		rob->mutable_dribbler()->set_speed(robots[i]->command().dribbleSpeed());
		for(int j = 0; j < robots[i]->wheels().size(); j ++) {
			CommandWheel* cw = rob->add_wheels();
			cw->set_speed(robots[i]->wheels()[j].speed());
		}
	}
	tot.SerializePartialToOstream(&out);
	string s = out.str();
	append(QByteArray(s.c_str(), s.size()));
}
