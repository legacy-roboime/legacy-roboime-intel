#include "Play.h"

using namespace LibIntelligence;
using namespace Plays;

Play::Play(QObject* parent, Team* team, Stage* stage)
	: QObject(parent)
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

/*const*/ Stage* Play::stage() const
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

//#include "Play.h"
//
//using namespace LibIntelligence;
//
////constructors/destructor:
//Play::Play(void) {
//	_init();
//}
//
//Play::Play(string name) {
//	_name = name;
//}
//
//Play::Play(Stage stage) {
//	_init();
//	_stage = stage;
//}
//
//Play::~Play(void) {
//	//FIXME: this is wrong
//	_play.remove(this);
//	size_t i = _i;
//	__i.push_back(i);
//	__i.sort();
//	//puts("DESTROYED!!!!!");
//}
//
////methods:
//void Play::_init(void) {
//	if(__i.size() > 0) {
//		_i = __i.front();
//		__i.pop_front();
//	} else {
//		_i = _play.size();
//	}
//	_play.push_back(this);
//	_stage;
//	_steps = 0;
//	_active = true;
//}
//
//void Play::_take_step() {
//	//TODO: implement
//}
//
//void Play::change_stage(Stage stage) {
//	//TODO: implement
//}
//
//void Play::set_global_stage(void) {
//	//TODO: implement
//}
//void Play::take_step(void) {
//	//TODO: implement
//}