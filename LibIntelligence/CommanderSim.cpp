#include "CommanderSim.h"
#include "Command.h"
#include "Robot.h"
#include <QTimer>
#include <QtNetwork>
#include <QHostAddress>
#include <QVector>
#include <iostream>
#include <sstream>
#include "messages_internal_command.pb.h"

#define M_2PI	6.2831853071795865

using namespace std;
using namespace LibIntelligence;

CommanderSim::CommanderSim(QObject* parent, char* address, quint16 port)
	: UdpServer(parent, address, port),
	Commander()
{
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(send()));
	//for(size_t n=0; n<5; n++) _robot.push_back(0);
	timer->start(1000.0/60.0); //frequencia sensor camera 	//10.
}

CommanderSim::~CommanderSim()
{
	delete timer;
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
	for(int n = 0; n < 5; n++) {
		if(robots[n]!=0) {
			Robot* r = robots[n];
			Command& c = r->command();
			//TODO: implement logging
			//QVector<qreal> w = c.wheelsSpeed();
			if(c.wheelsSize() != 4) {
				qCritical("prepare: Wrong number of wheels!");
				break;
			}

			out << c.wheelSpeedAt(0) * M_2PI << " "
				<< c.wheelSpeedAt(1) * M_2PI << " "
				<< c.wheelSpeedAt(2) * M_2PI << " "
				<< c.wheelSpeedAt(3) * M_2PI << " "
				<< c.dribbleSpeed()  << " "
				<< c.kickSpeed()     << " ";

			r->newCommand();
		} else {
			out << "0 0 0 0 0 0 ";
		}
	}
	out << endl;
	//cout << out.str();//DEBUG
	append(QByteArray(out.str().c_str(), out.str().size()));
}

