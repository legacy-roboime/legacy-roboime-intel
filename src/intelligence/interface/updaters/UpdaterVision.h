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
		UpdaterVision(QObject* parent=0, quint16 port=10002, const char* address="224.5.23.2");
		~UpdaterVision();
		//methods:
		void prepare();
		void receive();
		void step();
		bool timeout();
		int* framesSinceLastSeenBlue;
		int* framesSinceLastSeenYellow;
		// What cameras do you want?
		// 0 for cam0, 1 for cam1, 2 for both cams.
		void setWantedCam(int i) { wantedCam_ = i; }

	private slots:
		void receiveData();

	private:
		int wantedCam_;
		char cTimeout;
		QUdpSocket* udpSocket;
		QQueue<SSL_WrapperPacket*> packets;
		unsigned int itr_vision;

	};
}
