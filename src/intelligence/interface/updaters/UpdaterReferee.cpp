#include <QtNetwork>
#include "Update.h"
#include "UpdaterReferee.h"
#include "UpdateReferee.h"
#include "Stage.h"

//using namespace std;
using namespace LibIntelligence;

UpdaterReferee::UpdaterReferee(QObject* parent, char* address, int port) 
	: Updater(),
	cmd_counter_tmp(-1),
	packets(new QQueue<RefData>())
{
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
		RefData data;

		//store the packet: 
		uchar cmd_counter = (uchar)(packet[1]);

		if ( cmd_counter != cmd_counter_tmp ) 
		{ // se nao houver nenhum comando novo, fazer nada
			cmd_counter_tmp = cmd_counter;
			data.cmd = (char)packet[0];
			data.cmd_counter = (uchar)packet[1];
			data.goals_blue = (uchar)packet[2];
			data.goals_yellow = (uchar)packet[3];
			data.time_remaining = ((int)(packet[4]) << 8)  + (int)(packet[5]); //byte mais significativo
			packets->push_back(data);
		}
	}
}

void UpdaterReferee::receive() {}

void UpdaterReferee::prepare() {
	while(!packets->empty()){
		RefData& data = packets->front();
		enqueue(new UpdateReferee(data.cmd, data.cmd_counter, data.goals_blue, data.goals_yellow, data.time_remaining));
		packets->pop_front();
	}
}