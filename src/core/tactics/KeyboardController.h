#pragma once
#include "Robot.h"
namespace LibIntelligence{
	
	class KeyboardController{
	
	private:
		Robot* _controlled_robot;
		int _move_up_key;
		int _move_down_key;
		int _move_left_key;
		int _move_right_key;
		int _spin_clock_key;
		int _spin_counter_key;
		int _dribble_key;
		int _kick_key;
		double _movement_speed;
		double _angular_speed;

	public:
		KeyboardController();
		KeyboardController(Robot* robot, int keys[6], double spd, double angspd);
		~KeyboardController();

		//TODO: Usar a API pra enviar o comando
		void give_command();
			
	};
}