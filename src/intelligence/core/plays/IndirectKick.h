#ifndef INDIRECTKICK_H
#define INDIRECTKICK_H

#include "StopReferee.h"
#include "Tactics.h"
#include <QFile>
#include "SampledKick.h"

namespace LibIntelligence
{
	namespace Plays
	{

		class PlayStateMachine: public QObject{
			Q_OBJECT
			public:
				PlayStateMachine(unsigned int startState);
				void setState(unsigned int newState);
				unsigned int currentState(){return currentState_;}
				void step();
			private:
				quint16 oldState;
				quint16 currentState_;
				quint16 lastState;
				quint16 timeInState;
				QTime timeEntryState;
		};


		class IndirectKick : public StopReferee
		{
			Q_OBJECT

		public:
			IndirectKick(QObject *parent, Team*, Stage*, Robot* gkeeper, qreal speed);
			void step();
			void restart();
			bool finished();
		private:
			qreal speed;
			PlayStateMachine stateMachine;
			typedef enum StateName {START, GO_POSITION, PASS, TOUCHED, END};
			Goto* passedGoto;
			Tactics::Zickler43* attack;
			SampledKick* sk;
			Robot* passerRobot;
			Robot* passedRobot;
			Object bestPos;
		};

		
	}



}

#endif // INDIRECTKICK_H
