#include "Update.h"
//#include "timer.h"

using namespace LibIntelligence;

Update::Update(double tSent, double tCapture, int camId) {
	//_time_received = GetTimeSec();//TODO: fix this
	time_sent_ = tSent;
	time_capture_ = tCapture;
	camera_id_ = camId;
}

double Update::time_received() {
	return time_received_;
}

double Update::time_sent() {
	return time_sent_;
}

double Update::time_capture() {
	return time_capture_;
}

int Update::camera_id(){
	return camera_id_;
}