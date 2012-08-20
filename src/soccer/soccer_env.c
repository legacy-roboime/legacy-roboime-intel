#include "soccer_env.h"

static SoccerEnvironment senv;

void soccer_env_init( void )
{
 senv.sample_period = .015;
 senv.red_speed = 2.5;
 senv.blue_speed = 2.5;
 senv.red_dribble_speed = 1;
 senv.blue_dribble_speed = 1;
 senv.red_pass_speed = 4;
 senv.blue_pass_speed = 4;
 senv.red_pass_delay = 2.;
 senv.blue_pass_delay = 2.;
 senv.red_kick_delay = 1.;
 senv.blue_kick_delay = 0;
 senv.red_move_radius = 6.;
 senv.blue_move_radius = 6.;
 senv.red_recv_radius = .01;
 senv.blue_recv_radius = .01;
 senv.robot_radius = .09;
 senv.goal_size = .7;
 senv.max_red_kick_dist = .5;//12.;//
 senv.max_blue_kick_dist = .5;//12.;//
 senv.hfield_w = 3.025;// 1.325;//
 senv.hfield_h = 2.025;// 0.825;//
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


