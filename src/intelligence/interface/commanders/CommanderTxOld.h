#pragma once
#ifndef COMMANDERTXOLD_H
#define COMMANDERTXOLD_H

#include "LibIntelligence.h"
#include "UdpServer.h"
#include "Commander.h"

namespace LibIntelligence
{
	//Legacy commander Tx, will be removed eventually
	class CommanderTxOld : public UdpServer, public Commander
	{
		Q_OBJECT

	public:
		CommanderTxOld(QObject* parent=0, char* address="127.0.0.1", quint16 port=9050);
		~CommanderTxOld();
		
		void add(Robot*);
		void step();
	};
}

#endif // COMMANDERTXOLD_H
