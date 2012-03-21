#include "UpdaterSIMi.h"
#include "Update.h"
#include "UpdateBall.h"
#include "UpdateRobot.h"
#include "UpdateStageGeometry.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

//using namespace std;
using namespace LibIntelligence;

UpdaterSIMi::UpdaterSIMi(QObject* parent) : Updater(/*parent*/) {
}

UpdaterSIMi::~UpdaterSIMi() {}

void UpdaterSIMi::step() {
	prepare();
	//apply();//naah
}

void UpdaterSIMi::receive() {}

void UpdaterSIMi::addPacket(SSL_WrapperPacket* packet){
	packets.push_back(packet);
}

void UpdaterSIMi::prepare() {
	while(!packets.empty()){
		SSL_WrapperPacket packet = *(packets.front());
		if (packet.has_detection()) {
			SSL_DetectionFrame detection = packet.detection();
			//TODO: implement UpdateStage
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
		if(packet.has_geometry()){
			SSL_GeometryData geom = packet.geometry();
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

//#include "UpdaterSIMi.h"
////#include "Simulation.h"
//#include "Update.h"
//#include "UpdateBall.h"
//#include "UpdateRobot.h"
//#include "UpdaterSIM.h"
//
//using namespace std;
//using namespace LibIntelligence;
//
//UpdaterSIMi::UpdaterSIMi() : Updater() {
//	_scene = 0;
//}
//
//UpdaterSIMi::UpdaterSIMi(int i) : Updater() {
//	_scene = i;
//}
//
//UpdaterSIMi::~UpdaterSIMi() {}
//
//void UpdaterSIMi::receive() {
//	//TODO: implement
//}
