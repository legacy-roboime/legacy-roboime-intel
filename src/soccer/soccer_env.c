#include "soccer_env.h"

static SoccerEnvironment senv;

void soccer_env_init( void )
{
 senv.red_speed = 1.;
 senv.blue_speed = 1;
 senv.red_dribble_speed = .5;
 senv.blue_dribble_speed = .5;
 senv.red_pass_speed = 1.5;
 senv.blue_pass_speed = 1.5;
 senv.red_move_radius = 3.;
 senv.blue_move_radius = 3.;
 senv.red_recv_radius = .1;
 senv.blue_recv_radius = .1;
 senv.robot_radius = .1;
 senv.goal_size = .7;
 senv.max_red_kick_dist = 3.;
 senv.max_blue_kick_dist = 3.;
 senv.hfield_w = 3.0; 
 senv.hfield_h = 2.0; 
}

SoccerEnvironment* soccer_env(void)
{
 return &senv;
}

