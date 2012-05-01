#include "Sleep.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

Sleep::Sleep(QObject *p, Robot* r) : Skill(p, r) {}

void Sleep::step()
{
	//TODO: implementar, precisa de comando de sleep
}