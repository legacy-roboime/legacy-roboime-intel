#ifndef MACHINETRANSITION_H
#define MACHINETRANSITION_H

#include <QObject>

namespace LibIntelligence
{
	class State;

	class MachineTransition : public QObject
	{
		Q_OBJECT

	public:
		MachineTransition(QObject* parent = 0, State* source = 0, State* target = 0, qreal probability = 1.);
		~MachineTransition(void);
		virtual bool condition() = 0;
		qreal probability();
		State* getTarget();
		State* getSource();

	protected:
		State* source_; //otimizacao em cache, mas nao precisava ter a origem
		State* target_;

	private:
		qreal probability_;
	};
}

#endif //MACHINETRANSITION_H

