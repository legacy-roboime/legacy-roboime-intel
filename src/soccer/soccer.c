#include "soccer.h"

static float goal_hole_size( SoccerState *s, Vector2 src_point, Vector2 goal );
static void is_kick_scored( SoccerState *s, 
                                Vector2 src_point, Vector2 goal_point );

static Boolean is_inside_area( SoccerState *s, int robot, Vector2 p, Vector2 goal );

static float goal_covering( SoccerState *s );
static float goal_distance( SoccerState *s );


static float goal_covering( SoccerState *s );
static float goal_distance( SoccerState *s );


SoccerState* sstate_alloc( void )
{
 int i;
 SoccerState *s = NEWSTRUCT(SoccerState);
 s->real_state = NULL;  
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
 float t, s0 = 0, s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7=0, s8 = 0;
 float min_red_dist_to_ball, min_red_dist_to_blue_goal,
       min_red_dist_to_red_goal;
 float min_blue_dist_to_ball,  min_blue_dist_to_blue_goal, 
       min_blue_dist_to_red_goal;
 float ball_dist_to_red_goal, ball_dist_to_blue_goal;
 Vector2 blue_goal, red_goal;
 Vector2 real_ball;

 real_ball = s->real_state->ball;

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

 if( s->goal_received )
     s0 = -1000000 + 1000000*s->red_goal_covering; 
 if( s->goal_scored )
     s0 = 1000000 - 1000000*s->blue_goal_covering;

 s1 += 1000*ball_dist_to_red_goal;
 s1 -= 1000*ball_dist_to_blue_goal;

 if( sstate_min_red_dist( s->real_state, real_ball ) > 2 ) 
      s2 -= 1000*sstate_min_red_dist( s, v2_add( real_ball,  v2_scale( .5, v2_unit( v2_sub( real_ball, soccer_env()->blue_goal )) )));
 if( sstate_min_blue_dist( s->real_state, real_ball ) > 2 ) 
      s2 += 50*min_blue_dist_to_ball;

 s3 -= 100*min_red_dist_to_red_goal;
 s3 += 100*min_blue_dist_to_blue_goal;

 if(  s->red_ball_owner >=  0 )/*|| 
      (( t = sstate_time_to_red_get_ball( s, &closest_red ) <
       sstate_time_to_blue_get_ball( s, &closest_blue )) && ( t > 0 )) ) */
         s4 += 10000; 
 if(  s->blue_ball_owner >=  0 ) /* || 
      (( t= sstate_time_to_blue_get_ball( s, &closest_blue ) <
       sstate_time_to_blue_get_ball( s, &closest_red )) && ( t > 0 )) ) */
         s4 -= 10000; 

/* 
 if( (s->ball.x > -2.5) && (s->ball.x < 2.5 ) )
    s5 += 300*MAX(min_blue_dist_to_ball,3);
 else{
	if( s->ball
    s5 = 900;

 }
 if( (s->ball.x > -2.5) && (s->ball.x < 2.5 ) )
    s5 = -300*MAX(min_red_dist_to_ball,3);
 else
    s5 = -900;
	*/
 
 for( i = 0; i < NPLAYERS; i++){
   if( (sstate_min_red_dist( s->real_state, real_ball ) < .3 )  && 
	   (sstate_min_blue_dist( s->real_state, real_ball ) >  .6 ) && 
       sstate_possible_red_pass(s, i, v2_add(s->red[i],v2_make(-.01,0) )) > 0){ 
           s6 += (100 - //10*v2_norm( v2_sub( s->red[i], blue_goal ))
                     // -40*SQR( v2_norm( v2_sub( s->red[i], s->ball )) - 1. )
                     // + 30*sstate_min_blue_dist( s, s->red[i] )
					  + 150*goal_hole_size( s, s->red[i], soccer_env()->blue_goal )
                 );
   }
   if( ( sstate_min_blue_dist( s->real_state, real_ball ) < .3)  &&
	    (sstate_min_red_dist( s->real_state, real_ball ) >  .6 ) && 
       sstate_possible_blue_pass(s, i, v2_add(s->blue[i],v2_make(.01,0) )) > 0){
           s6 += (-100 +// 10*v2_norm( v2_sub( s->blue[i], red_goal ))
                      // +40*SQR( v2_norm( v2_sub( s->blue[i], s->ball )) - 1. )
                      // -30*sstate_min_red_dist( s, s->blue[i] ) 
					   -150*goal_hole_size( s, s->blue[i], soccer_env()->red_goal )
                 ); 
   }
 } 

 
 //  if( sstate_min_red_dist( s, s->ball );ball_owner >= 0 )
 //     s7 += 100000*goal_hole_size( s, s->ball, soccer_env()->blue_goal );
 //  else
//	  s7 += 10000*goal_hole_size( s, s->ball, soccer_env()->blue_goal ); 
   if( sstate_min_blue_dist( s->real_state, real_ball ) < .5   ) 
      s7 -= 800000*goal_hole_size( s, s->ball, soccer_env()->red_goal );
   if( (sstate_min_red_dist( s->real_state, real_ball ) < .3 ) && ( sstate_min_blue_dist( s->real_state, real_ball ) > .3)    )
      s7 += 400000*goal_hole_size( s, s->ball, soccer_env()->blue_goal );

 //  else
 //     s7 -= 20000*goal_hole_size( s,  real_ball, soccer_env()->red_goal );

 //  for( i = 0; i < NPLAYERS; i++){
//      if( is_inside_area( s, i, s->red[i], soccer_env()->red_goal ))
 //        s8 = -900000;
 //  }

  //  if( sstate_min_blue_dist( s->real_state, real_ball ) < .5 ) 
  //    s8 -= 100000*goal_hole_size( s, real_ball, soccer_env()->red_goal );


 // for( i = 0; i < NPLAYERS; i++){
    // if( s->red_ball_owner < 0 
	//   s8 += 10*goal_hole_size( s, s->red[i], soccer_env()->blue_goal );
  //   if( s->blue_ball_owner < 0  )
    //   s8 -= 10*goal_hole_size( s, s->blue[i], soccer_env()->red_goal );
 // }
 

 return s0 + s1 + s2 + s3 + s4 + s6 +  s7;/*s0 + s1 + s2 + s4 + s6 +  s7 + s8; //s0 + s1 + s2 + s3 + s4 + s6 + s7; // + s6; //+ s8; //s0 +  s1 + s2 /*+ s3 + s4 + s6 + */ + s7; //  +  s3 + s4 + s5 + s6 + s7;// + s8;// + s3 + s4 + s5 + s6 + s7 + s8;*/
} 


Boolean sstate_is_valid_red_pos( SoccerState *s, int robot, Vector2 p )
{
 int i;
 float diameter = 2*soccer_env()->robot_radius;

 if( sstate_is_inside_field( s, p )/*||  !sstate_is_inside_field( s, s->red[robot] )*/ ){
   for( i = 0; i < NPLAYERS; i++ ){
      if( (i != robot) && (v2_norm( v2_sub( p, s->red[i] )) < diameter) ){
         return FALSE;
      }
      if( v2_norm( v2_sub( p, s->blue[i] )) < diameter )
        return FALSE;  
	  if( is_inside_area( s, robot, p, soccer_env()->red_goal ) && (robot != 0) )
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

 if( sstate_is_inside_field( s, p ) /*||  !sstate_is_inside_field( s, s->blue[robot] )*/ ){
   for( i = 0; i < NPLAYERS; i++ ){
      if( v2_norm( v2_sub( p, s->red[i] )) < diameter )
        return FALSE;
      if(  (i != robot) && (v2_norm( v2_sub( p, s->blue[i] )) < diameter) )
        return FALSE; 
	  if( is_inside_area( s, robot, p, soccer_env()->blue_goal ) && (robot != 0) )
	    return FALSE;  
    }
    return TRUE; 
 }
 else
    return FALSE; 
}


Boolean is_inside_area( SoccerState *s, int robot, Vector2 p, Vector2 goal )
{
 Vector2 l,r, displ, aux, orthog, rel_p;

 displ = v2_make( 0, (3./8.)*soccer_env()->goal_size);
 l = v2_sub( goal, displ ); 
 r = v2_add( goal, displ );
 aux = v2_unit( v2_sub(l,r) );
 rel_p = v2_sub(p, goal );
 orthog = v2_sub( rel_p, v2_scale( v2_dot(aux,rel_p), aux) );


 if( (v2_norm( v2_sub(p, l)) < .8  ) ||  (v2_norm( v2_sub(p, r)) < .8  ) ||
	  v2_norm( orthog ) < .8 )
    return TRUE;
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
          p.y = p.y + k + DRAND()*dy;
          is_kick_scored(s, src_point, p );
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

