#ifndef AUTORETALIATE_H
#define AUTORETALIATE_H

#include "Play.h"
#include "Tactics.h"
#include <QFile>

namespace LibIntelligence
{
	namespace Plays
	{
		class AutoRetaliate : public Play
		{
			Q_OBJECT

		public:
			AutoRetaliate(QObject *parent, Team*, Stage*, Robot* gkeeper, qreal speed);
			void step();
		};
	}
}

#endif // CBR2011_H
