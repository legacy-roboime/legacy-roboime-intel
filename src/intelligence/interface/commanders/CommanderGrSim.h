#pragma once
#ifndef COMMANDERGRSIM_H
#define COMMANDERGRSIM_H

#include "LibIntelligence.h"
#include "UdpServer.h"
#include "Commander.h"

namespace LibIntelligence
{
	class CommanderGrSim : public UdpServer, public Commander
	{
		Q_OBJECT
		
	public:
        CommanderGrSim(QObject* parent=0, quint16 port=20011, const char* address="127.0.0.1");

		void step();
	};
}

#endif // COMMANDERGRSIM_H
