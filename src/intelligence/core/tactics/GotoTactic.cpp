#include "GotoTactic.h"

using namespace LibIntelligence;
using namespace Tactics;

GotoTactic::GotoTactic(QObject *p, Robot* r)
	: Tactic(p, r),
	goto_(new Goto(this, r))
{}


GotoTactic::~GotoTactic(void){
	delete goto_;
}

void GotoTactic::step(){
	goto_->step();
}