#include <QtNetwork>
#include "Update.h"
#include "UpdaterReferee.h"
#include "UpdateReferee.h"
#include "Stage.h"

//using namespace std;
using namespace LibIntelligence;

UpdaterReferee::UpdaterReferee(QObject* parent, const char* address, int port) : Updater() {
	QHostAddress groupAddress = QHostAddress(address);

	udpSocket = new QUdpSocket(this);
	udpSocket->bind(port, QUdpSocket::ShareAddress);
	udpSocket->joinMulticastGroup(groupAddress);

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

UpdaterReferee::~UpdaterReferee() {
	delete udpSocket;
}

void UpdaterReferee::step() {
	prepare();
}

void UpdaterReferee::receiveData() {
	//cout << "Helloo there!!" << endl;
	while (udpSocket->hasPendingDatagrams()) {
		//receive a packet:
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(datagram.data(), datagram.size());
		char* packet = datagram.data();
		//store the packet:
		unsigned char cmd_counter = (unsigned char)packet[1];

		static unsigned char cmd_counter_tmp = -1;
		if ( cmd_counter != cmd_counter_tmp ) 
		{ // se nao houver nenhum comando novo, fazer nada
			cmd_counter_tmp = cmd_counter;
			packets.push_back(packet);

			Stage::setCmdReferee((char)packet[0]);
		}
	}
}

void UpdaterReferee::receive() {}

void UpdaterReferee::prepare() {
	while(!packets.empty()){
		char* packet = packets.front();
		char cmd_tmp = (char)packet[0];
		unsigned char cmd_counter = (unsigned char)packet[1];
		unsigned char goals_blue = (unsigned char)packet[2];
		unsigned char goals_yellow = (unsigned char)packet[3];
		int time_remaining = ((int)(packet[4]) << 8)  + (int)(packet[5]); //byte mais significativo
		enqueue(new UpdateReferee(cmd_tmp, cmd_counter, goals_blue, goals_yellow, time_remaining));
		packets.pop_front();
	}
}