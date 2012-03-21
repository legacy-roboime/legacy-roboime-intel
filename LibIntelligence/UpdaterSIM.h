#pragma once
#include "LibIntelligence.h"
#include <deque>
#include "Updater.h"
#include "CommanderSIM.h"
//#include "UDPClientThread.h"

namespace LibIntelligence {

	class UpdaterSIM : public Updater {
		friend class CommanderSIM;

	private:
		//static UDPClientThread* _udpclient;//Use the same server as CommanderSIM when possible
		deque<string> _queue;

	public:
		UpdaterSIM();
		UpdaterSIM(string address, unsigned short port);
		~UpdaterSIM();

		//methods:
		void receive();
		void prepare();

	};
}