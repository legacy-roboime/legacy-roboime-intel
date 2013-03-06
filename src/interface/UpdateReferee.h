#pragma once

#include "Update.h"
#include "Updater.h"
#include <string>

namespace LibIntelligence {

	class UpdateReferee : public Update
	{
	private:
		char cmd;                      // current referee command
		unsigned char cmd_counter;     // increments each time a new command is set
		unsigned char goals_blue;      // current score for blue team
		unsigned char goals_yellow;    // current score for yellow team
		int time_remaining;			   // seconds remaining for current game stage (network byte order)
	public:
		UpdateReferee(char cmd, unsigned char cmd_counter, unsigned char goals_blue, unsigned char goals_yellow, int time_remaining);

		void apply(Updater*);			// go through the updaters list and apply the update
		std::string to_string();
	};
}