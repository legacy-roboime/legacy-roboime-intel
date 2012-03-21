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
		MachineTransition(QObject* parent = 0, QObject* metaObj = 0, const char* method = 0, State* source = 0, State* target = 0, qreal probability = 1.);
		~MachineTransition(void);
		bool condition();
		qreal probability();
		State* getTarget();

	private:
		State* source_; //otimizacao em cache, mas nao precisava ter a origem
		State* target_;
		qreal probability_;
		QObject* metaObj_;
		const char* method_;
	};
}

#endif //MACHINETRANSITION_H

