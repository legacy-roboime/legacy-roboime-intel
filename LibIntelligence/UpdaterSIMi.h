#pragma once
#include "LibIntelligence.h"
#include "Updater.h"
#include <QQueue>

class SSL_WrapperPacket;

namespace LibIntelligence
{
	class UpdaterSIMi : public Updater
	{
		Q_OBJECT

	public:
		UpdaterSIMi(QObject* parent=0);
		~UpdaterSIMi();
		//methods:
		void prepare();
		void receive();
		void step();
		void addPacket(SSL_WrapperPacket* packet);

	private:
		QQueue<SSL_WrapperPacket*> packets;
	};
}

//#pragma once
//#include "LibIntelligence.h"
//#include <deque>
//#include "Updater.h"
//#include "CommanderSIMi.h"
//
//namespace LibIntelligence {
//
//	class UpdaterSIMi : public Updater {
//		friend class CommanderSIMi;
//
//	private:
//		int _scene;
//
//	public:
//		UpdaterSIMi();//scena padrão 0
//		UpdaterSIMi(int);
//		~UpdaterSIMi();
//
//		//methods:
//		void receive();
//
//	};
//}