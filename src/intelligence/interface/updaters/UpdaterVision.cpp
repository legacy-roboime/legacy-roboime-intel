#include <QtNetwork>

#include "UpdaterVision.h"
#include "Update.h"
#include "UpdateBall.h"
#include "UpdateRobot.h"
#include "UpdateStageGeometry.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

//using namespace std;
using namespace LibIntelligence;

UpdaterVision::UpdaterVision(QObject* parent, char* address, quint16 port) : Updater() {
	QHostAddress groupAddress = QHostAddress(address);

	udpSocket = new QUdpSocket(this);
    udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	udpSocket->joinMulticastGroup(groupAddress);
	//udpSocket->bind(

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
	//udpSocket->open(
	//udpSocket->open(
}

UpdaterVision::~UpdaterVision() {}

void UpdaterVision::step() {
	prepare();
	//apply();//naah
}

void UpdaterVision::receiveData() {
	//cout << "Helloo there!!" << endl;
	while (udpSocket->hasPendingDatagrams()) {
		//receive a packet:
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());
		udpSocket->readDatagram(datagram.data(), datagram.size());
		//parse the packet:
		SSL_WrapperPacket* packet = new SSL_WrapperPacket();
		packet->ParseFromArray(datagram.data(), datagram.size());
		//store the packet:
		packets.push_back(packet);
		//packets.enqueue(packet);//this is the same, though not STL compatible
	}
}

void UpdaterVision::receive() {}

void UpdaterVision::prepare() {
	while(!packets.empty()){
		SSL_WrapperPacket* packet = packets.front();
		if (packet->has_detection()) {
			SSL_DetectionFrame detection = packet->detection();
			//double t_now = GetTimeSec();
			//printf("Camera ID=%d FRAME=%d T_CAPTURE=%.4f\n",detection.camera_id(),detection.frame_number(),detection.t_capture());
			//printf("SSL-Vision Processing Latency                   %7.3fms\n",(detection.t_sent()-detection.t_capture())*1000.0);
			//printf("Network Latency (assuming synched system clock) %7.3fms\n",(t_now-detection.t_sent())*1000.0);
			//printf("Total Latency   (assuming synched system clock) %7.3fms\n",(t_now-detection.t_capture())*1000.0);
			double t_capture = detection.t_capture();
			double t_sent = detection.t_sent();

			for (int i = 0; i < detection.balls_size(); i++)
				enqueue(new UpdateBall(detection.balls(i), t_sent, t_capture, detection.camera_id()));

			for (int i = 0; i < detection.robots_blue_size(); i++)
				enqueue(new UpdateRobot(BLUE, detection.robots_blue(i), t_sent, t_capture, detection.camera_id()));

			for (int i = 0; i < detection.robots_yellow_size(); i++)
				enqueue(new UpdateRobot(YELLOW, detection.robots_yellow(i), t_sent, t_capture, detection.camera_id()));

		}
		if(packet->has_geometry()){
			SSL_GeometryData geom = packet->geometry();
			SSL_GeometryFieldSize fieldSize = geom.field();
			enqueue(new UpdateStageGeometry(geom));
			/*int calib_n = geom.calib_size();
			for (int i=0; i< calib_n; i++) {
				if(geom.calib(i).camera_id()==0){ //geom.id == 0 
					
				}
				else if(geom.calib(i).camera_id()==1){ //geom.id == 1
					
				}
			}*/
		}
		packets.pop_front();
	}
}