#include "CommanderTxOld.h"
#include "Command.h"
#include "Robot.h"
#include <iostream>
#include <sstream>
#include <QtNetwork>
#include <QVector>
#include "messages_internal_command.pb.h"

using namespace std;
using namespace LibIntelligence;

CommanderTxOld::CommanderTxOld(QObject* parent, char* address, quint16 port)
	: UdpServer(parent, address, port),
	Commander()
{
	connect(parent, SIGNAL(readySend()), this, SLOT(send()));
	for(size_t n=0; n<5; n++) robots.push_back(0);
}

CommanderTxOld::~CommanderTxOld()
{
}

void CommanderTxOld::add(Robot* r)
{
	int i = r->id();
	if(i<5) {
		if(robots[i]!=0) delete robots[i];
		robots[i] = r;
	} else {
		cerr << "Warning: index greater than 4 not supported" << endl;
	}
}

void CommanderTxOld::step()
{
	stringstream out;
	for(size_t n=0; n<5; n++) {
		if(robots[n]!=0) {
			Robot* r = robots[n];
			Command& c = r->command();
			//TODO: implement logging
			//QVector<qreal> w = c.wheelsSpeed();
			if(c.wheelsSize() == 2) {
				qreal k = c.kickSpeed();
				qreal d = c.dribbleSpeed();
				out << c.wheelSpeedAt(0) << " "
					<< c.wheelSpeedAt(1) << " "
					<< 0.0               << " "
					<< 0.0               << " "
					<< d << " " << k << " ";
				//r->newCommand();
			} else if(c.wheelsSize() == 4) {
				qreal k = c.kickSpeed();
				qreal d = c.dribbleSpeed();
				out << c.wheelSpeedAt(0) << " "
					<< c.wheelSpeedAt(1) << " "
					<< c.wheelSpeedAt(2) << " "
					<< c.wheelSpeedAt(3) << " "
					<< d << " " << k << " ";
				//r->newCommand();
			} else {
				qCritical("prepare: Wrong number of wheels!");
				break;
			}
		} else {
			//out << "0 0 0 0 0 0 ";
		}
	}
	out << endl;
	//cout << out.str();
	append(QByteArray(out.str().c_str(), out.str().size()));
}

