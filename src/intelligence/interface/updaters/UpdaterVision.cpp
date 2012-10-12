#include <QtNetwork>

#include "UpdaterVision.h"
#include "Update.h"
#include "UpdateBall.h"
#include "UpdateRobot.h"
#include "UpdateStageGeometry.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#define MAX_NB_PATTERNS 12
#define TIMEOUT 100

//using namespace std;
using namespace LibIntelligence;

UpdaterVision::UpdaterVision(QObject* parent, quint16 port, const char* address) : Updater() {
	QHostAddress groupAddress = QHostAddress(address);

	udpSocket = new QUdpSocket(this);
	udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
	udpSocket->joinMulticastGroup(groupAddress);

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
	
	itr_vision = 0;

	cTimeout = 0;
}

UpdaterVision::~UpdaterVision() 
{
	delete udpSocket;
}

void UpdaterVision::step() {
	prepare();
	//apply();//naah
}

bool UpdaterVision::timeout()
{
	if(this->cTimeout>TIMEOUT)
		return true;
	else
		return false;
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
	cTimeout++;
	if(cTimeout>TIMEOUT)
		cTimeout=TIMEOUT+1;
	while(!packets.empty()){
		cTimeout= 0;
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

			for (int i = 0; i < detection.balls_size(); i++){
				//if(detection.camera_id() == 0)
					enqueue(new UpdateBall(detection.balls(i), t_sent, t_capture, detection.camera_id()));
			}

			//REFRESH VISION
			if(itr_vision>50){
				itr_vision=0;

				SSL_DetectionRobot null_robot = SSL_DetectionRobot();
				null_robot.set_x(-10000.0);
				null_robot.set_y(-10000.0);
				null_robot.set_orientation(0);

				for(int i=0; i<MAX_NB_PATTERNS; i++){
					null_robot.set_robot_id(i);
					enqueue(new UpdateRobot(BLUE, null_robot, t_sent, t_capture, detection.camera_id(), false));
					enqueue(new UpdateRobot(YELLOW, null_robot, t_sent, t_capture, detection.camera_id(), false));
				}
			}
			itr_vision++;

			for (int i = 0; i < detection.robots_blue_size(); i++)
				enqueue(new UpdateRobot(BLUE, detection.robots_blue(i), t_sent, t_capture, detection.camera_id(), true));

			for (int i = 0; i < detection.robots_yellow_size(); i++)
				enqueue(new UpdateRobot(YELLOW, detection.robots_yellow(i), t_sent, t_capture, detection.camera_id(), true));

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
		delete packet;
	}
}