#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <QObject>
#include "State.h"
#include <QVector>

namespace LibIntelligence
{
	class StateMachine : public QObject //essa maquina nao aceita estados paralelamente sendo executados
	{
		Q_OBJECT

	public:
		StateMachine(QObject* parent = 0, bool deterministic = false);
		StateMachine(QObject* parent, const StateMachine& machine);
		~StateMachine(void);
		State* getStateByName(const QString& name);
		State* getCurrentState();
		State* getFinalState();
		State* getInitialState();
		bool getDeterministic();
		QVector<State*>* getStates();
		void setCurrentState(State* current);
		void pushState(State* state);
		void execute(); //avança a máquina de estado no tempo
		void createTransition(QObject* metaObj = 0, const char* method = 0, State* source = 0, State* target = 0, qreal probability = 1.);
		void setInitialState(State* initial);
		void setFinalState(State* final);
		void reset(); //set o current para initial
		bool busy(); //a maquina esta ocupada, ou seja, ela nao vai mudar de estado corrente

	protected:
		QVector<State*> states_;
		State* initial_;
		State* current_;

	private:
		State* final_;
		bool deterministic_;
		//QVector<MachineTransition*> transitions;
	};
}

#endif //STATEMACHINE_H
