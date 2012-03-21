#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QVector>

namespace LibIntelligence
{
	class MachineTransition;

	class State : public QObject
	{
		Q_OBJECT

	public:
		State(QObject* parent = 0);
		State(const State& state);
		~State(void);
		void pushTransition(MachineTransition* transition); //transicao saindo desse estado
		virtual bool busy() = 0; //o estado esta ocupado, ou seja, a maquina que contem esse estado nao poder ir para uma das targets
		int sizeTransitions();
		MachineTransition* getTransition(int pos);

	private:
		QVector<MachineTransition*> outs_; //transicao saindo desse estado
	};
}

#endif //STATE_H