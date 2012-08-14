//#ifndef MAINPLAY_H
#if FALSE
#define MAINPLAY_H

#include "LibIntelligence.h"
#include <QObject>
#include "Play.h"
#include "Tactics.h"
#include "Plays.h"

namespace LibIntelligence
{
	class StopReferee;
	
	namespace Plays
	{
		class MainPlay : public Play
		{
			Q_OBJECT

		public:
			MainPlay(QObject*, Team*,Stage*);
			~MainPlay();

		protected:
			virtual void step();

		private:
			StopReferee* referee_;
			Tactics::Attacker* attacker_;
			Tactics::Blocker* blocker_[3];
			Tactics::Defender* defender_;
			Tactics::Goalkeeper* goalKeeper_;
		};
	}
}

#endif // STOPREFEREE_H
