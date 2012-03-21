#include "Battery.h"

using namespace LibIntelligence;

Battery::Battery(const Battery& b) : Component(b.parent()) {}

Battery::Battery(QObject *p) : Component(p) {}
