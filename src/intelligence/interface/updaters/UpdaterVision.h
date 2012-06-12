#pragma once
#include "LibIntelligence.h"
#include "Updater.h"
#include <QQueue>

//#include "robocup_ssl_client.h"

class QUdpSocket;
class SSL_WrapperPacket;

namespace LibIntelligence
{
	class UpdaterVision : public Updater
	{
		Q_OBJECT

	public:
		UpdaterVision(QObject* parent=0, quint16 port=10002, char* address="224.5.23.2");
		~UpdaterVision();
		//methods:
		void prepare();
		void receive();
		void step();

	private slots:
		void receiveData();

	private:
		QUdpSocket* udpSocket;
		QQueue<SSL_WrapperPacket*> packets;
	};
}