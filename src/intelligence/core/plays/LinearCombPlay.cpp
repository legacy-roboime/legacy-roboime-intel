#include "LinearCombPlay.h"

using namespace LibIntelligence;
using namespace Plays;
using namespace Tactics;

LinearCombPlay::LinearCombPlay(QObject *parent, Team* team, Stage* stage)
	: Play(parent, team, stage)
{
	player_[0] = new LinearCombTactic(this, team->at(0));
	player_[1] = new LinearCombTactic(this, team->at(1));
	player_[2] = new LinearCombTactic(this, team->at(2));
	player_[3] = new LinearCombTactic(this, team->at(3));
	player_[4] = new LinearCombTactic(this, team->at(4));
}

void LinearCombPlay::setCoefs( qreal *c0, qreal *c1, qreal *c2,
	                           qreal *c3, qreal *c4 )
{
 ((LinearCombTactic*)player_[0])->setCoefs(c0);
 ((LinearCombTactic*)player_[1])->setCoefs(c1);
 ((LinearCombTactic*)player_[2])->setCoefs(c2);
 ((LinearCombTactic*)player_[3])->setCoefs(c3);
 ((LinearCombTactic*)player_[4])->setCoefs(c4);

  
}

void LinearCombPlay::step()
{
 player_[0]->step();
 player_[1]->step();
 player_[2]->step();
 player_[3]->step();
 player_[4]->step(); 
}