#ifndef MULTICONTROLLER_H
#define MULTICONTROLLER_H

#include "Play.h"

namespace LibIntelligence
{
	namespace Plays
	{
		class MultiController : public Play
		{
			Q_OBJECT

		public:
			MultiController(QObject *parent);
			~MultiController();

		private:
	
		};
	}
}

#endif // MULTICONTROLLER_H
