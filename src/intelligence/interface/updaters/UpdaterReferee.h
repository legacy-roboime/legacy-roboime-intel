#pragma once
#include "LibIntelligence.h"
#include <deque>
#include "Updater.h"
#include <sstream>
#include <string>
#include <QQueue>

class QUdpSocket;

//using namespace std;

namespace LibIntelligence
{
	class UpdaterReferee : public Updater
	{
		Q_OBJECT

	public:
		UpdaterReferee(QObject* parent, const char* address="224.5.23.1", int port=10001);
		~UpdaterReferee();
		//methods:
		void prepare();
		void receive();
		void step();

	private slots:
		void receiveData();

	private:
		QUdpSocket* udpSocket;
		QQueue<char*> packets;
	};
}