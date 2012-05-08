#include "soccer.h"


SoccerState* sstate_alloc( void )
{
 int i;
 SoccerState *s = NEWSTRUCT(SoccerState);
 srand ( time(NULL) );
 sstate_restart_game_pos(s); 
 return s;
}


void sstate_restart_game_pos( SoccerState *s )
{
 int i;
 s->goal_scored = FALSE;
 s->goal_received = FALSE;
 s->ball_vel = v2_make(0,0); 
 s->red_ball_owner = -1;
 s->blue_ball_owner = -1;
 s->red_passer = -1;
 s->blue_passer = -1;

 s->ball = v2_make((2*DRAND()-1.)*soccer_env()->hfield_w,
                   (2*DRAND() -1.)*soccer_env()->hfield_h ); 

 for( i = 0; i < NPLAYERS; i++ ){
   s->red[i] = v2_make( DRAND()*soccer_env()->hfield_w,
                        (2*DRAND() -1.)*soccer_env()->hfield_h ); 
   s->blue[i] = v2_make((DRAND() -1)*soccer_env()->hfield_w,
                        (2*DRAND() -1.)*soccer_env()->hfield_h ); 
 }
}


float sstate_evaluate( SoccerState *s )
{
 float s1 = 0, s2 = 0, s3 = 0, s4 = 0;

 if( s->goal_scored )
   return MAX_FLOAT - 10000*s->blue_goal_covering;
 if( s->goal_received )
   return -MAX_FLOAT + 10000*s->red_goal_covering;

 if( s->red_ball_owner >=  0 )
   s1 += 1000; 
 if( s->blue_ball_owner >=  0 )
   s1 -= 1000; 

 s2 -= 100*v2_norm( v2_sub( s->ball, 
               v2_make( -soccer_env()->hfield_w, 0 ) ) );
 s2 += 100*v2_norm( v2_sub( s->ball, 
               v2_make( soccer_env()->hfield_w, 0 ) ) );

// if(  sstate_closest_red(s,s->ball) != s->red_passer )
      s3 -= sstate_min_red_dist( s, s->ball );
 //else
  //    s3 = -100; 

 //if(  sstate_closest_blue(s,s->ball) != s->blue_passer )
      s3 += sstate_min_blue_dist( s, s->ball );
// else
 //     s3 = 100;

 if( (s->ball.x > -2.5) && (s->ball.x < 2.5 ) )
    s4 = 300*MAX(sstate_min_blue_dist( s, s->ball ),3);
 else
  //  if(s->ball.x < -2.5) 
      s4 = 900;


 if( (s->ball.x > -2.5) && (s->ball.x < 2.5 ) )
    s4 = -300*MAX(sstate_min_red_dist( s, s->ball ),3);
 else
//    if(s->ball.x > 2.5) 
      s4 = -900;



 return s1 + s2 + s3 + s4;
} 


Boolean sstate_is_inside_field( SoccerState *s, Vector2 p )
{
 return (fabs(p.x)<= soccer_env()->hfield_w) && 
        (fabs(p.y)<= soccer_env()->hfield_h) ? TRUE : FALSE;
}


float time_to_intersect( Vector2 friend_pos, Vector2 friend_direction,
                         Vector2 enemy_pos, float friend_speed, float enemy_speed )
{
 float a, b, c, delta;
 Vector2 r;

 if( friend_speed < EPS )
   return v2_norm( v2_sub( enemy_pos, friend_pos ) )/
          enemy_speed; 

 r = v2_unit( friend_direction );
 a = ( SQR(friend_speed) - SQR(enemy_speed) );
 b = 2*v2_dot( v2_scale( friend_speed, r ),
               v2_sub(friend_pos,enemy_pos) );
 c = v2_sqrnorm(v2_sub(friend_pos,enemy_pos));
 delta = SQR(b) - 4*a*c;

 if( fabs(a) < EPS )
     return -c/b; 

 if( delta > EPS )
     return (-b - sqrt(delta))/(2*a);
 else
    return MAX_FLOAT;
}


