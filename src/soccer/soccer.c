#include "soccer.h"

static float goal_hole_size( SoccerState *s, Vector2 src_point, Vector2 goal );
static void is_kick_scored( SoccerState *s, 
                                Vector2 src_point, Vector2 goal_point );

static float goal_covering( SoccerState *s );
static float goal_distance( SoccerState *s );


static float goal_covering( SoccerState *s );
static float goal_distance( SoccerState *s );


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

 s->blue[0] = soccer_env()->blue_goal;
 s->red[0] = soccer_env()->red_goal;

 if( soccer_env()->left_red_side ){
   for( i = 1; i < NPLAYERS; i++ ){
       s->red[i] = v2_make( DRAND()*soccer_env()->hfield_w,
                            (2*DRAND() -1.)*soccer_env()->hfield_h ); 
       s->blue[i] = v2_make((DRAND() -1)*soccer_env()->hfield_w,
                            (2*DRAND() -1.)*soccer_env()->hfield_h ); 
   }
 }
 else{
   for( i = 1; i < NPLAYERS; i++ ){
     s->blue[i] = v2_make( DRAND()*soccer_env()->hfield_w,
                          (2*DRAND() -1.)*soccer_env()->hfield_h ); 
     s->red[i] = v2_make((DRAND() -1)*soccer_env()->hfield_w,
                        (2*DRAND() -1.)*soccer_env()->hfield_h ); 
   }
 } 
}


float sstate_evaluate( SoccerState *s )
{
 int i, closest_red, closest_blue;
 float t, s0 = 0, s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7=0;
 float min_red_dist_to_ball, min_red_dist_to_blue_goal,
       min_red_dist_to_red_goal;
 float min_blue_dist_to_ball,  min_blue_dist_to_blue_goal, 
       min_blue_dist_to_red_goal;
 float ball_dist_to_red_goal, ball_dist_to_blue_goal;
 Vector2 blue_goal, red_goal;

 blue_goal = soccer_env()->blue_goal;
 red_goal = soccer_env()->red_goal;
  
 min_red_dist_to_ball = sstate_min_red_dist( s, s->ball );
 min_red_dist_to_blue_goal = sstate_min_red_dist( s, blue_goal );
 min_red_dist_to_red_goal = sstate_min_red_dist( s, red_goal );

 min_blue_dist_to_ball = sstate_min_blue_dist( s, s->ball );
 min_blue_dist_to_blue_goal = sstate_min_blue_dist( s, blue_goal ); 
 min_blue_dist_to_red_goal = sstate_min_blue_dist( s, red_goal );

 ball_dist_to_red_goal = v2_norm( v2_sub( s->ball, red_goal ) );
 ball_dist_to_blue_goal = v2_norm( v2_sub( s->ball, blue_goal ) );

 if( s->goal_received ){
   if( s->red_goal_covering > .7 )
     s0 = -80000 + 80000*s->red_goal_covering; 
   else
     return -MAX_FLOAT;
 }
 if( s->goal_scored ){
   if( s->blue_goal_covering > .7 )
     s0 = 80000 - 80000*s->blue_goal_covering;
   else
     return MAX_FLOAT;
 }

 s1 += 300*ball_dist_to_red_goal;
 s1 -= 300*ball_dist_to_blue_goal;

 s2 -= 50*min_red_dist_to_ball;
 s2 += 50*min_blue_dist_to_ball;

 s3 -= 500*min_red_dist_to_red_goal;
 s3 += 500*min_blue_dist_to_blue_goal;

 if(  s->red_ball_owner >=  0 || 
      (( t = sstate_time_to_red_get_ball( s, &closest_red ) <
       sstate_time_to_blue_get_ball( s, &closest_blue )) && ( t > 0 )) )
         s4 += 1000; 
 if(  s->blue_ball_owner >=  0 || 
      (( t= sstate_time_to_blue_get_ball( s, &closest_blue ) <
       sstate_time_to_blue_get_ball( s, &closest_red )) && ( t > 0 )) )
         s4 -= 1000; 


 if( (s->ball.x > -2.5) && (s->ball.x < 2.5 ) )
    s5 = 300*MAX(min_blue_dist_to_ball,3);
 else
    s5 = 900;

 if( (s->ball.x > -2.5) && (s->ball.x < 2.5 ) )
    s5 = -300*MAX(min_red_dist_to_ball,3);
 else
    s5 = -900;

 for( i = 0; i < NPLAYERS; i++){
   if( (s->red_ball_owner >= 0 ) && 
       sstate_possible_red_pass(s, i, v2_add(s->red[i],v2_make(-.1,0) )) > 0){ 
           s6 += (400 - 40*v2_norm( v2_sub( s->red[i], blue_goal ))
                      +20*v2_norm( v2_sub( s->red[i], s->ball ))
                      + 5*sstate_min_blue_dist( s, s->red[i] )
                 );
   }
   if( ( s->blue_ball_owner >= 0 ) &&
       sstate_possible_blue_pass(s, i, v2_add(s->blue[i],v2_make(.1,0) )) > 0){
           s6 += (-400 + 40*v2_norm( v2_sub( s->blue[i], red_goal ))
                       -20*v2_norm( v2_sub( s->blue[i], s->ball ))
                       -5*sstate_min_red_dist( s, s->blue[i] )
                 ); 
   }
 } 

 
   if( min_red_dist_to_ball < 2 )
      s7 += 100000*goal_hole_size( s, s->ball, soccer_env()->blue_goal );
   if( min_blue_dist_to_ball < 2 )
      s7 -= 100000*goal_hole_size( s, s->ball, soccer_env()->red_goal );

 return s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7;
} 


Boolean sstate_is_valid_red_pos( SoccerState *s, int robot, Vector2 p )
{
 int i;
 float diameter = 2*soccer_env()->robot_radius;

 if( sstate_is_inside_field( s, p ) ){
   for( i = 0; i < NPLAYERS; i++ ){
      if( (i != robot) && (v2_norm( v2_sub( p, s->red[i] )) < diameter) ){
         return FALSE;
      }
      if( v2_norm( v2_sub( p, s->blue[i] )) < diameter )
        return FALSE;  
    }
    return TRUE; 
 }
 else
    return FALSE; 
}


Boolean sstate_is_valid_blue_pos( SoccerState *s, int robot, Vector2 p )
{
 int i;
 float diameter = 2*soccer_env()->robot_radius;

 if( sstate_is_inside_field( s, p ) ){
   for( i = 0; i < NPLAYERS; i++ ){
      if( v2_norm( v2_sub( p, s->red[i] )) < diameter )
        return FALSE;
      if(  (i != robot) && (v2_norm( v2_sub( p, s->blue[i] )) < diameter) )
        return FALSE;  
    }
    return TRUE; 
 }
 else
    return FALSE; 
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


float goal_hole_size( SoccerState *s, Vector2 src_point, Vector2 goal )
{
 float dy = .15;
 float k, hole_size = 0;
 Vector2 p;

 for( k = -.5*soccer_env()->goal_size; k < .5*soccer_env()->goal_size; k += dy ){ 
          p = goal;
          p.y = p.y + k;
          is_kick_scored(s, src_point, p );
          if( s->goal_scored )
              hole_size += dy;
 }
 return hole_size;
}


static void is_kick_scored( SoccerState *s, Vector2 src_point, Vector2 goal_point )
{
 int i;
 Boolean goal_scored;
 float dotprod_red, dotprod_blue;
 Vector2 goal_direction, red_relpos, blue_relpos, proj_red, proj_blue;

 goal_scored = TRUE;
 for( i = 0; i < NPLAYERS; i++ ){
       goal_direction = v2_unit( v2_sub( goal_point,  src_point) );
       blue_relpos = v2_sub( s->blue[i],  src_point );
       red_relpos = v2_sub( s->red[i],  src_point );
       proj_blue = v2_scale( dotprod_blue = v2_dot( blue_relpos, goal_direction ), 
                        goal_direction );
       proj_red = v2_scale( dotprod_red = v2_dot( red_relpos, goal_direction ), 
                        goal_direction );
       if( (dotprod_red > 0 ) && sstate_is_inside_field( s, s->red[i] ) &&
           (v2_norm( v2_sub( red_relpos, proj_red )) < soccer_env()->robot_radius)  )
              goal_scored = FALSE;  
       if( (dotprod_blue > 0 ) && sstate_is_inside_field( s, s->blue[i] ) &&
           (v2_norm( v2_sub( blue_relpos, proj_blue )) < soccer_env()->robot_radius)  )
              goal_scored = FALSE;  
     }
 return goal_scored;
}

