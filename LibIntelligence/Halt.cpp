#include "Halt.h"
#include "Skills.h"

using namespace LibIntelligence;
using namespace Plays;

Halt::Halt(QObject* parent, Team* team, Stage* stage)
	: Play(parent, team, stage) {
	for(int i = 0; i < team_->size(); i++){
			wait_[i] = new Skills::Wait(this, team_->at(i));
	}
}

void Halt::step()
{	
	for(int i = 0; i < team_->size(); i++){
			wait_[i]->step();
	}
}