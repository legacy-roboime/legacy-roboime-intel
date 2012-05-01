#include "soccer.h"


Boolean sstate_is_inside_field( SoccerState *s, Vector2 p )
{
 return (fabs(p.x)<= s->field_w) && 
        (fabs(p.y)<= s->field_h) ? TRUE : FALSE;
}


float sstate_min_red_dist( SoccerState *s, Vector2 p )
{
 int i;
 float min_sqrdist = MAX_FLOAT; 

 for( i = 0; i < NPLAYERS; i++ )
    if( min_sqrdist > v2_sqrnorm( v2_sub(p, s->red[i])) )
        min_sqrdist = v2_sqrnorm( v2_sub(p, s->red[i]) );

 return sqrt( min_sqrdist );    
}


float sstate_min_blue_dist( SoccerState *s, Vector2 p )
{
 int i;
 float min_sqrdist = MAX_FLOAT; 

 for( i = 0; i < NPLAYERS; i++ )
    if( min_sqrdist > v2_sqrnorm( v2_sub(p, s->blue[i])) )
        min_sqrdist = v2_sqrnorm( v2_sub(p, s->blue[i]) );

 return sqrt( min_sqrdist );    
}


int sstate_closest_red( SoccerState *s, Vector2 p )
{
 int i, closest;
 float min_sqrdist = MAX_FLOAT; 

 for( i = 0; i < NPLAYERS; i++ )
    if( min_sqrdist > v2_sqrnorm( v2_sub(p, s->red[i])) ){
        min_sqrdist = v2_sqrnorm( v2_sub(p, s->red[i]) );
        closest = i;
    }

 return closest;    
}


int sstate_closest_blue( SoccerState *s, Vector2 p )
{
 int i, closest;
 float min_sqrdist = MAX_FLOAT; 

 for( i = 0; i < NPLAYERS; i++ )
    if( min_sqrdist > v2_sqrnorm( v2_sub(p, s->blue[i])) ){
        min_sqrdist = v2_sqrnorm( v2_sub(p, s->blue[i]) );
        closest = i;
    }

 return closest;  
}


float sstate_time_to_red_get_ball( SoccerState *s, int *closest )
{
 int i;
 float t, mint;

 mint = MAX_FLOAT;
 for( i = 0; i < NPLAYERS; i++ ){ 
   t = time_to_intersect( s->ball, s->ball_vel, s->red[i], 
                          v2_norm( s->ball_vel ), s->red_speed );
   if( t < mint ){
      mint = t;
      if( closest != NULL ) 
         *closest = i;
   } 
 }
 return mint;
}


float sstate_time_to_blue_get_ball( SoccerState *s, int *closest )
{
 int i;
 float t, mint;

 mint = MAX_FLOAT;
 for( i = 0; i < NPLAYERS; i++ ){ 
   t = time_to_intersect( s->ball, s->ball_vel, s->blue[i], 
                          v2_norm( s->ball_vel ), s->blue_speed );
   if( t < mint ){
      mint = t;
      if( closest != NULL )
         *closest = i;
   } 
 }
 return mint;
}


float sstate_possible_red_pass( SoccerState *s, int recv, Vector2 p )
{
 int i;
 float t1, t2, t_move, t_pass;
 Vector2 pass_dir, move_dir;

 if( !sstate_is_inside_field(s,p) )
   return -1;

 pass_dir = v2_sub(p,s->ball);
 move_dir = v2_sub(p,s->red[recv]);
 t_move = v2_norm(v2_sub( s->red[recv], p ))*s->red_speed;
 t_pass = v2_norm(v2_sub( s->red[ s->red_ball_owner ], p ))*s->red_pass_speed;

 for( i = 0; i < NPLAYERS; i++ ){
    t1 = time_to_intersect( s->ball, pass_dir, s->blue[i], s->red_pass_speed, s->blue_speed );
    t2 = time_to_intersect( s->red[recv], move_dir, s->blue[i], s->red_speed, s->blue_speed );

    if( (( t1 > 0 ) && (t1 < t_pass)) ||  (( t2 > 0 ) && (t2 < t_move )))
       return -1;
 }

 if( t_move < t_pass )
    return t_pass;
 else
    return -1; 
}


float sstate_possible_blue_pass( SoccerState *s, int recv, Vector2 p )
{
 int i;
 float t1, t2, t_move, t_pass;
 Vector2 pass_dir, move_dir;

 if( !sstate_is_inside_field(s,p) )
   return -1;

 pass_dir = v2_sub(p,s->ball);
 move_dir = v2_sub(p,s->blue[recv]);
 t_move = v2_norm(v2_sub( s->blue[recv], p ))*s->blue_speed;
 t_pass = v2_norm(v2_sub( s->blue[ s->blue_ball_owner ], p ))*s->blue_pass_speed;

 for( i = 0; i < NPLAYERS; i++ ){
    t1 = time_to_intersect( s->ball, pass_dir, s->red[i], s->blue_pass_speed, s->red_speed );
    t2 = time_to_intersect( s->blue[recv], move_dir, s->red[i], s->blue_speed, s->red_speed );

    if( (( t1 > 0 ) && (t1 < t_pass)) ||  (( t2 > 0 ) && (t2 < t_move )))
       return -1;
 }

 if( t_move < t_pass )
    return t_pass;
 else
    return -1; 
}


float time_to_intersect( Vector2 friend_, Vector2 friend_direction,
                         Vector2 enemy, float friend_speed, float enemy_speed )
{
 float a, b, c, k, delta;
 Vector2 r;

 if( friend_speed < EPS )
   return v2_norm( v2_sub( enemy, friend_ ) )/
          enemy_speed; 
 
 r = v2_unit( friend_direction );
 k = enemy_speed/friend_speed;
 a = ( 1 - SQR(k));
 b = 2*v2_dot( r, v2_sub(friend_,enemy) );
 c = v2_sqrnorm(v2_sub(friend_,enemy));
 delta = SQR(b) - 4*a*c;

 if( fabs(a) < EPS )
     return -c/b; 

 if( delta > EPS )
       return (-b - sqrt(delta))/(2*a);
 else
    return MAX_FLOAT;
}





