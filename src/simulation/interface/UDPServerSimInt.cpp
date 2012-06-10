#include "UDPServerSimInt.h"
#include "Simulation.h"
#include <QtNetwork>
#include <QTime>
#include "config.h"

UDPServerSimInt::UDPServerSimInt(QObject* parent, Simulation* simulation, char* address, quint16 port) : QObject(parent)
{
	QHostAddress groupAddress = QHostAddress(address);
	this->simulation = simulation;
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(port, QUdpSocket::ShareAddress);
	udpSocket->joinMulticastGroup(groupAddress);
	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(parsing()));
}

UDPServerSimInt::~UDPServerSimInt(void)
{
	delete udpSocket;
	delete simulation;
}

void UDPServerSimInt::setSimulation(Simulation* simulation)
{
	this->simulation = simulation;
}

void UDPServerSimInt::parsing()
{
	QTime time = QTime::currentTime();
#ifdef DEBUG_UDP
	static double lTime = 0;
	double mSec = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec() - lTime;
	lTime = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec();
	Simulation::cout << "Receive time: " << mSec << endl;
#endif

	while(udpSocket->hasPendingDatagrams()){
		QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);
		
		//simulation->append(QString(datagram.data()));
		string sendString = simulation->parseLegacyString(string(datagram.data()));
		//udpSocket->writeDatagram(sendString.c_str(), sendString.length(), sender, senderPort);
	}
}