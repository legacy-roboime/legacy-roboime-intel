#include "Filter.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Filters;

Filter::Filter(QObject* p, Updates* u)
	: QObject(p),
	updates_(),
	updates_from(u) {}

void Filter::setUpdatesFrom(Updates* u)
{
	updates_from = u;
}

Updates* Filter::updates()
{
	return &updates_;
}

