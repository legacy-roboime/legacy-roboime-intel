//#include "UpdaterSIM.h"
//#include "Update.h"
//#include "UpdateBall.h"
//#include "UpdateRobot.h"
//#include "UpdaterSIM.h"
//
//using namespace std;
//
//using namespace LibIntelligence;
//
//UDPClientThread* UpdaterSIM::_udpclient = 0;
//
//UpdaterSIM::UpdaterSIM() : Updater() {
//	if(_udpclient==0) {
//		if(CommanderSIM::_udpclient!=0) {
//			_udpclient = CommanderSIM::_udpclient;
//		} else {
//			_udpclient = new UDPClientThread("_udpclient", 9876, "127.0.0.1");
//			_udpclient->start();
//		}
//	}
//}
//
//UpdaterSIM::UpdaterSIM(string address, unsigned short port) : Updater() {
//	delete _udpclient;
//	_udpclient = new UDPClientThread("_udpclient", port, address);
//}
//
//UpdaterSIM::~UpdaterSIM() {
//	delete _udpclient;
//}
//
//void UpdaterSIM::receive() {
//	_udpclient->setSendString("1 0\n");
//	_udpclient->run();
//	_queue.push_back(_udpclient->getLastReceivedString());
//}
//
//void UpdaterSIM::prepare() {
//	while(!_queue.empty()){
//		stringstream in = stringstream(_queue.front());
//		//TODO: validation
//		real x, y, a;
//		in >> x;
//		in >> y;
//		_update.push_back(new UpdateBall(x,y));
//		for(int k=1; k<=10; k++) {
//			in >> x;
//			in >> y;
//			in >> a;
//			_update.push_back(new UpdateRobot(k,x,y,a));
//		}
//		_queue.pop_front();
//	}
//}