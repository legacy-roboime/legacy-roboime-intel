#pragma once
#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class QTimer;
class QByteArray;

#include "messages_robocup_ssl_refbox_log.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

class Simulation;

class UDPMulticastSenderSSLVision : public QObject
{
	Q_OBJECT

private:
	QUdpSocket* udpSocket;
	QTimer* timer;
	QHostAddress groupAddress;
	Simulation* simulation;
	quint16 port;

private slots:
     void sendDatagram();

public:
	void setTTL(int newTtl);
	UDPMulticastSenderSSLVision(QObject* parent=0, Simulation* simulation=0, char* address="224.5.23.2", quint16 port=10002);
	~UDPMulticastSenderSSLVision(void);
	QByteArray buildSendMessage();
	void startSending();
	void stopSending();
};
