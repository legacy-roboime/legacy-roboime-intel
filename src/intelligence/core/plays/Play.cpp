#include "Play.h"

using namespace LibIntelligence;
using namespace Plays;

Play::Play(QObject* parent, Team* team, Stage* stage)
	: QThread(parent)
{
	stage_ = stage;
	team_ = team;
}

Play::~Play(){
	delete team_;
	delete stage_;
    for( int i = 0; i < team_->size(); i++ )
		delete player_[i];
}

void Play::setStage(Stage* s)
{
	stage_ = s;
}

const Stage* Play::stage() const
{
	return stage_;
}

Stage* Play::stage()
{
	return stage_;
}
	
void Play::setTeam(Team* t)
{
	team_ = t;
}

const Team* Play::team() const
{
	return team_;
}

Team* Play::team()
{
	return team_;
}