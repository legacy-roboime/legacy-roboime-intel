#include "UpdateReferee.h"
#include "Stage.h"
#include "Team.h"
#include <iostream>
#include <sstream>

using namespace LibIntelligence;
using namespace std;

UpdateReferee::UpdateReferee(char cmd, unsigned char cmd_counter, unsigned char goals_blue, unsigned char goals_yellow, int time_remaining) : Update() {
	this->cmd = cmd;
	this->cmd_counter = cmd_counter;
	this->goals_blue = goals_blue;
	this->goals_yellow = goals_yellow;
	this->time_remaining = time_remaining;
}

UpdateReferee::~UpdateReferee(void) {}

void UpdateReferee::apply(Updater* u) {
	//cout << to_string() << endl;
	for(size_t k=u->stagesSize(); k>0; k--) {
		u->stage(k)->setTimeLeft(this->time_remaining);
		u->stage(k)->setCmdReferee(cmd);
		u->stage(k)->blueTeam()->setGoals(this->goals_blue);
		u->stage(k)->yellowTeam()->setGoals(this->goals_yellow);
	}
}

string UpdateReferee::to_string() {
	stringstream out;
	out << "UpdateReferee [" << cmd << cmd_counter << goals_blue << goals_yellow << time_remaining << "]";
	return out.str();
}
