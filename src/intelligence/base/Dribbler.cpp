#include "Dribbler.h"

using namespace LibIntelligence;

Dribbler::Dribbler(const Dribbler& d) : Actuator(d.parent(), d.speed()) {}

Dribbler::Dribbler(QObject* p, qreal s) : Actuator(p, s) {}
