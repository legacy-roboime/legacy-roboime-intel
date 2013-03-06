#include "CommanderSim.h"
#include "Command.h"
#include "Robot.h"
#include <QtNetwork>
#include <QHostAddress>
#include <QVector>
#include <iostream>
#include <sstream>
#include "messages_internal_command.pb.h"
#include "config.h"

#define M_2PI	6.2831853071795865

using namespace std;
using namespace LibIntelligence;

CommanderSim::CommanderSim(QObject* parent, const char* address, quint16 port)
	: UdpServer(parent, address, port),
	Commander()
{
	this->udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
	//for(size_t n=0; n<NPLAYERS; n++) _robot.push_back(0);
}

CommanderSim::~CommanderSim()
{
}

/*void CommanderSim::add(Robot* r)
{
int i = r->id();
if(i<5) {
if(_robot[i] != 0) delete _robot[i];
_robot[i] = r;
} else {
cerr << "Warning: index greater than 4 not supported" << endl;
}
}*/

void CommanderSim::step()
{
	stringstream out;
	out << "15 0 ";
	//GAMBIARRA
	if(robots[0]->color() == YELLOW) out << "0 ";
	else out << "1 ";
	for(int n = 0; n < NPLAYERS; n++) {
		if(robots[n]!=0) {
			Robot* r = robots[n];
			Command& c = r->command();
			//TODO: implement logging
			//QVector<qreal> w = c.wheelsSpeed();
			if(c.wheelsSize() != 4) {
				qCritical("prepare: Wrong number of wheels!");
				break;
			}

			out << c.wheelSpeedAt(0) << " "
				<< c.wheelSpeedAt(1) << " "
				<< c.wheelSpeedAt(2) << " "
				<< c.wheelSpeedAt(3) << " "
				<< c.dribbleSpeed()  << " "
				<< c.kickSpeed()     << " ";

			r->newCommand();
		} else {
			out << "0 0 0 0 0 0 ";
		}
	}
	out << endl;
	//std::cout << "SIM: " << out.str() << endl;//DEBUG
	append(QByteArray(out.str().c_str(), out.str().size()));
}

