#include "soccer_env.h"

static SoccerEnvironment senv;

void soccer_env_init( void )
{
 senv.sample_period = .07;
 senv.red_speed = 3.;
 senv.blue_speed = 3.;
 senv.red_dribble_speed = .5;
 senv.blue_dribble_speed = .5;
 senv.red_pass_speed = 3.5;
 senv.blue_pass_speed = 3.5;
 senv.red_move_radius = 6.;
 senv.blue_move_radius = 1;
 senv.red_recv_radius = .1;
 senv.blue_recv_radius = .1;
 senv.robot_radius = .09;
 senv.goal_size = .7;
 senv.max_red_kick_dist = 3.;
 senv.max_blue_kick_dist = 3.;
 senv.hfield_w = 3.025; 
 senv.hfield_h = 2.025; 
 senv.red_goal = v2_make( senv.hfield_w, 0 );
 senv.blue_goal = v2_make( -senv.hfield_w, 0 );
 senv.left_red_side = TRUE;
}


void soccer_env_red_side( int side )
{
 if( side == RIGHT ){
   senv.left_red_side = TRUE;
   senv.red_goal = v2_make( senv.hfield_w, 0 );
   senv.blue_goal = v2_make( -senv.hfield_w, 0 );
 }
 else{
   senv.left_red_side = FALSE;
   senv.red_goal = v2_make( -senv.hfield_w, 0 );
   senv.blue_goal = v2_make( senv.hfield_w, 0 );
 }
}


SoccerEnvironment* soccer_env(void)
{
 return &senv;
}


