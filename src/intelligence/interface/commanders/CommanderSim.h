#pragma once
#ifndef COMMANDERSIM_H
#define COMMANDERSIM_H

#include "LibIntelligence.h"
#include "UdpServer.h"
#include "Commander.h"

namespace LibIntelligence
{
	class CommanderSim : public UdpServer, public Commander
	{
		Q_OBJECT
		
	public:
		CommanderSim(QObject* parent=0, char* address="224.5.23.3", quint16 port=9876);
		~CommanderSim();

		//void add(Robot*);
		void step();
	};
}

#endif // COMMANDERSIM_H
