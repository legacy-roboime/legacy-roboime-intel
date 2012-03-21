#include "UDPMulticastSenderSSLVision.h"
#include "Simulation.h"
#include <QtNetwork>
#include <QTimer>

UDPMulticastSenderSSLVision::UDPMulticastSenderSSLVision(QObject* parent, Simulation* simulation, char* address, quint16 port): QObject(parent), port(port)
{
	this->simulation = simulation;
	this->groupAddress = QHostAddress(address);
	timer = new QTimer(this);
	udpSocket = new QUdpSocket(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(sendDatagram()));
	udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
	startSending();
}

UDPMulticastSenderSSLVision::~UDPMulticastSenderSSLVision(void)
{
	delete udpSocket;
	delete timer;
	delete simulation;
}

void UDPMulticastSenderSSLVision::startSending(){
	timer->start(1000./60.); //camera 60 fps
}

void UDPMulticastSenderSSLVision::stopSending(){
	timer->stop();
}

void UDPMulticastSenderSSLVision::setTTL(int newTtl)
{
	udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, newTtl);
}

QByteArray UDPMulticastSenderSSLVision::buildSendMessage()
{
	QByteArray datagram;

	SSL_WrapperPacket wrapperPacket = simulation->getSSLWrapper(simulation->gBaseScene);

	static const int ECHOMAX = 2048;//512 // Longest string to echo
	char sendBuffer[ECHOMAX];			  // message to send
	//string buffer;
	//setBufLen(wrapperPacket.ByteSize());
	wrapperPacket.SerializePartialToArray(/*datagram.data()*/sendBuffer, wrapperPacket.ByteSize());
	//char* data = datagram.data();
	//data = sendBuffer;
	datagram = QByteArray(sendBuffer, wrapperPacket.ByteSize());

	//SSL_WrapperPacket packet;
	//packet.ParseFromString(buffer);

	// Optional:  Delete all global objects allocated by libprotobuf.
	//google::protobuf::ShutdownProtobufLibrary();

	return datagram;
}

void UDPMulticastSenderSSLVision::sendDatagram()
{
	QByteArray datagram = buildSendMessage();

	udpSocket->writeDatagram(datagram.data(), datagram.size(), groupAddress, port);
}