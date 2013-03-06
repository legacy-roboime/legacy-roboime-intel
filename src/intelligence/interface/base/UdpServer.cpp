#include "UdpServer.h"
#include <QtNetwork>

UdpServer::UdpServer(QObject* parent, const char* address, quint16 port)
	: QObject(parent),
	udpSocket(new QUdpSocket(this)),
	address(new QHostAddress(address)),
	port(port) {}

UdpServer::~UdpServer()
{
	delete udpSocket;
	delete address;
}

void UdpServer::append(const QByteArray& d)
{
	queue.enqueue(d);
}

void UdpServer::send()
{
	//QTime time = QTime::currentTime();
	//static double lTime = 0;
	//double mSec = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec() - lTime;
	//printf("ENV %f\n",mSec);
	//lTime = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec();

	//while(!queue.empty()) {
	//	//NOTE: Notice the low reliability by not checking if the data was sent
	//	QByteArray& datagram = queue.dequeue();
	//	udpSocket->writeDatagram(datagram.data(), datagram.size(), *address, port);
	//}

	if(!queue.isEmpty()){
        QByteArray& datagram(queue.last());
		udpSocket->writeDatagram(datagram, *address, port);
		queue.clear();
	}
}
