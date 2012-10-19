#pragma once
#ifndef COMMANDER_H
#define COMMANDER_H

#include "LibIntelligence.h"
#include <QQueue>

namespace LibIntelligence
{
	class Robot;
	class Command;

	class Commander
	{
	public:
		Commander();
		virtual ~Commander();

		//methods:
		virtual void add(Robot*);
		virtual void del(Robot*);
		virtual void prepare();
        //virtual void send();
		virtual void step();

	protected:
		//fields:
		QQueue<Robot*> robots;
		QQueue<Command*> logs;
	};
}

#endif // COMMANDER_H
