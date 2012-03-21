#pragma once
#ifndef UPDATE_H
#define UPDATE_H

#include "LibIntelligence.h"

namespace LibIntelligence
{
	class Updater;

	class Update
	{
	public:
		Update(double timeSent=0.0, double timeCapture=0.0, int camId=0);

		virtual void apply(Updater*) = 0;//apply an update through an updater
		double time_sent();
		double time_received();
		double time_capture();
		int camera_id();

	protected:
		double time_received_;
		double time_sent_;
		double time_capture_;
		int camera_id_;
	};
}

#endif // UPDATE_H