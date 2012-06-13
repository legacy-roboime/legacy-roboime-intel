#pragma once
#include "LibIntelligence.h"
#include <list>
#include <string.h>
#include "Stage.h"
#include "Tactic.h"
#include <QThread>

using namespace std;

namespace LibIntelligence
{
	namespace Plays
	{	
		class Play : public QThread
		{
			Q_OBJECT

		public:
			Play(QObject*, Team*, Stage*);
			~Play(void);

			void setStage(Stage*);
			const Stage* stage() const;
			Stage* stage();
			
			void setTeam(Team*);
			const Team* team() const;
			Team* team();

			virtual void step() = 0;

		protected:
			Play(void);//must always be called on the constructor

			Stage* stage_;
			Team* team_;
			Tactics::Tactic* player_[5];
		};
	}
}