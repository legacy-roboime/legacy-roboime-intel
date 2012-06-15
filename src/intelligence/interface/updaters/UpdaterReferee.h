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
	struct RefData{
		char cmd;
		uchar cmd_counter;
		uchar goals_blue;
		uchar goals_yellow;
		int time_remaining;
	};

	class UpdaterReferee : public Updater
	{
		Q_OBJECT

	public:
		UpdaterReferee(QObject* parent, char* address="224.5.23.1", int port=10001);
		~UpdaterReferee();
		//methods:
		void prepare();
		void receive();
		void step();

	private slots:
		void receiveData();

	private:
		uchar cmd_counter_tmp;
		QUdpSocket* udpSocket;
		QQueue<RefData>* packets;
	};
}