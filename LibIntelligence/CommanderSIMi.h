#pragma once
#include "LibIntelligence.h"
#include <deque>
#include "Commander.h"
#include "UpdaterSIMi.h"
//#include "Simulation.h"

namespace LibIntelligence {

	class CommanderSIMi : public Commander {
		friend class UpdaterSIMi;

	private:
		int _scene;

	public:
		CommanderSIMi();
		CommanderSIMi(int);
		~CommanderSIMi();

		//methods:
		void send();

	};
}