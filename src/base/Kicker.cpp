#include "Kicker.h"

using namespace LibIntelligence;

Kicker::Kicker(const Kicker& k) : Actuator(k.parent(), k.speed()) {}

Kicker::Kicker(const Kicker* k) : Actuator(k->parent(), k->speed()) {}

Kicker::Kicker(QObject* p, qreal s) : Actuator(p, s) {}
