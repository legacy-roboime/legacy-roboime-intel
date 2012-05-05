#include "soccer_env.h"

static SoccerEnvironment senv;

void soccer_env_init( void )
{
 senv.red_speed = 3000;
 senv.blue_speed = 3000;
 senv.red_dribble_speed = 1000;
 senv.blue_dribble_speed = 1000;
 senv.red_pass_speed = 800;
 senv.blue_pass_speed = 800;
 senv.robot_radius = 90;
 senv.goal_size = 700;
 senv.field_w = 6050; 
 senv.field_h = 4050; 
 senv.left_red_side = TRUE;
}

SoccerEnvironment* soccer_env(void)
{
 return &senv;
}

