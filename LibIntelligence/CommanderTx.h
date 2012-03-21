#pragma once
#ifndef COMMANDERTX_H
#define COMMANDERTX_H

#include "LibIntelligence.h"
#include "UdpServer.h"
#include "Commander.h"

class QTimer;

namespace LibIntelligence
{
	class CommanderTx : public UdpServer, public Commander
	{
		//Q_OBJECT

	public:
		CommanderTx(QObject* parent=0, char* address="127.0.0.1", quint16 port=9050);
		~CommanderTx();

		void step();

	private:
		QTimer* timer;
	};
}

#endif // COMMANDERTX_H
