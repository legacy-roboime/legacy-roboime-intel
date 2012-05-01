#include "Updates.h"
#include "Update.h"

using namespace LibIntelligence;

void Updates::apply(Updater* u)
{
	while(!empty()) dequeue()->apply(u);
}