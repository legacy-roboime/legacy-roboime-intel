#include "KeyboardController.h"
#include <windows.h>

using namespace LibIntelligence;

KeyboardController::KeyboardController(Robot* robot, int key[8], double spd, double angspd){
	_controlled_robot=robot;
	_move_up_key=key[0];
	_move_down_key=key[1];
	_move_left_key=key[2];
	_move_right_key=key[3];
	_spin_clock_key=key[4];
	_spin_counter_key=key[5];	
	_dribble_key=key[6];
	_kick_key=key[7];
	_movement_speed=spd;
	_angular_speed=angspd;
	
}

KeyboardController::KeyboardController(){}

KeyboardController::~KeyboardController(){}
