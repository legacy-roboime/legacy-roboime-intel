#pragma once
#ifndef UPDATES_H
#define UPDATES_H

#include "LibIntelligence.h"
#include <QQueue>

namespace LibIntelligence
{
	class Update;
	class Updater;

	class Updates : public QQueue<Update*>
	{
	public:
		void apply(Updater*);
	};
}

#endif // UPDATES_H
