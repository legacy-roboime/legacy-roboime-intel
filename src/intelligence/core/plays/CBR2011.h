#ifndef CBR2011_H
#define CBR2011_H

#include "Plays.h"
#include "Tactics.h"
#include <QFile>

namespace LibIntelligence
{
	namespace Plays
	{
		class CBR2011 : public Play
		{
			Q_OBJECT

		public:
			CBR2011(QObject *parent, Team* team ,Stage* stage);
			~CBR2011();
			void step();

		private:
			QFile log; 
		};
	}
}

#endif // CBR2011_H
