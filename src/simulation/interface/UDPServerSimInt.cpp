#include "UDPServerSimInt.h"
#include "Simulation.h"
#include <QtNetwork>
#include <QTime>

UDPServerSimInt::UDPServerSimInt(QObject* parent, Simulation* simulation, quint16 port) : QObject(parent)
{
	this->simulation = simulation;
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(port);
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
	//QTime time = QTime::currentTime();
	//static double lTime = 0;
	//double mSec = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec() - lTime;
	//printf("REC %f\n",mSec);
	//lTime = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec();

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