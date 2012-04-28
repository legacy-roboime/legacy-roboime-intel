#include "soccer.h"

/*
#define DEBUG(s)            printf(s) 
#define DEBUG2(s,a,b)       printf(s,a,b)
#define DEBUG3(s,a,b,c)     printf(s,a,b,c)
#define DEBUG4(s,a,b,c,d)   printf(s,a,b,c,d)
#define DEBUG5(s,a,b,c,d,e) printf(s,a,b,c,d,e)
*/

#define DEBUG(s)             ;
#define DEBUG2(s,a,b)        ;
#define DEBUG3(s,a,b,c)      ;
#define DEBUG4(s,a,b,c,d)    ;
#define DEBUG5(s,a,b,c,d,e)  ;

static void is_red_kick_scored( SoccerState *s, Vector2 goal );
static void is_blue_kick_scored( SoccerState *s, Vector2 goal );


SoccerState* sstate_alloc( void )
{
 int i;
 SoccerState *s = NEWSTRUCT(SoccerState);

 s->ball = v2_make(0,0);
 s->ball_vel = v2_make(0,0);
 s->goal_scored = FALSE;
 s->goal_received = FALSE;
 s->left_red_side = TRUE;
 s->red_ball_owner = -1;
 s->blue_ball_owner = -1;
 s->red_speed = 1000;
 s->blue_speed = 1000;
 s->red_dribble_speed = 500;
 s->blue_dribble_speed = 500;
 s->red_pass_speed = 800;
 s->blue_pass_speed = 800;
 s->robot_radius = 90;//100;
 s->goal_size = 700;
 s->field_w = 8000;//6000; 
 s->field_h = 6000;//4000; 

 srand ( time(NULL) );

 for( i = 0; i < NPLAYERS; i++ ){
   s->red[i] = v2_make( (DRAND()-.5)*s->field_w,
                        (DRAND()-.5)*s->field_h ); 
   s->blue[i] = v2_make( (DRAND()-.5)*s->field_w,
                         (DRAND()-.5)*s->field_h ); 
 }

 return s;
}


float sstate_evaluate( SoccerState *s )
{
 float s1 = 0, s2 = 0, s3 = 0, s4 = 0;

 if( s->goal_scored )
   return MAX_FLOAT;

 if( s->goal_received )
   return -MAX_FLOAT;

 if( s->red_ball_owner >=  0 )
   s1 += 1000; 

 if( s->blue_ball_owner >=  0 )
   s1 -= 1000; 

 if(s->left_red_side)
   s2 = -0.1*v2_norm( v2_sub( s->ball, v2_make( +s->field_w, 0 ) ) );
 else
   s2 = -0.1*v2_norm( v2_sub( s->ball, v2_make( -s->field_w, 0 ) ) );

 s3 = -0.001*sstate_min_red_dist( s, s->ball );

 if( (s->ball.x > -2500) && (s->ball.x < 2500 ) )
    s4 = 0.3*MAX(sstate_min_blue_dist( s, s->ball ),3000.);
 else
    if(s->ball.x < -2500) 
      s4 = 900;

 return s1 + s2 + s3 + s4;
} 


void sstate_restart_game_pos( SoccerState *s )
{
 int i;

 s->goal_scored = FALSE;
 s->goal_received = FALSE;
 s->ball = v2_make(0,0);
 s->red_ball_owner = -1;
 s->blue_ball_owner = -1;

 for( i = 0; i < NPLAYERS; i++ ){
   s->red[i] = v2_make( (DRAND()-.5)*s->field_w,
                        (DRAND()-.5)*s->field_h ); 
   s->blue[i] = v2_make( (DRAND()-.5)*s->field_w,
                         (DRAND()-.5)*s->field_h ); 
 }
}


SoccerAction sstate_red_get_ball( SoccerState *s )
{
 float t;
 int closest_red, closest_blue;
 SoccerAction action = saction_make(s->red_ball_owner);

 if( (s->red_ball_owner < 0) && (s->blue_ball_owner < 0) ){
   if( (t =  sstate_time_to_red_get_ball( s, &closest_red )) <
       sstate_time_to_blue_get_ball( s, &closest_blue ) && ( t > 0 ) ){
          DEBUG("red get ball\n");  
        s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );
        s->ball_vel = v2_make(0,0);
        action.move[closest_red] = v2_sub( s->ball, s->red[closest_red] );
		//printf("%f %f\n",action.move[closest_red].x, action.move[closest_red].y);
        s->red[closest_red] = s->ball;
        s->red_ball_owner = closest_red;  
        action.ball_owner = closest_red;
		
   }
 }
 return action;
}


SoccerAction sstate_red_receive_ball( SoccerState *s, int recv )
{
 float t, time_to_blue_block, time_to_receive;
 int closest_blue;
 SoccerAction action = saction_make(s->red_ball_owner);

 if( (s->red_ball_owner >= 0) || (s->blue_ball_owner >= 0) )
    return action;
 
 time_to_blue_block = sstate_time_to_blue_get_ball( s, &closest_blue ); 
 time_to_receive = time_to_intersect( s->ball, s->ball_vel, s->red[recv], 
                                      v2_norm( s->ball_vel ), s->red_speed );

 if( (time_to_receive > 0 ) && (time_to_blue_block > time_to_receive) ){
   t = DRAND()*( time_to_blue_block - time_to_receive ) + time_to_receive;
   s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );  
   s->ball_vel = v2_make(0,0);
   action.move[recv] = v2_sub( s->ball, s->red[recv] );
   s->red[recv] = s->ball;
   s->red_ball_owner = recv;  
   action.ball_owner = recv;
 }
 return action;
}
  

void sstate_blue_get_ball( SoccerState *s )
{
 float t;
 int closest_red, closest_blue;

 if( (s->red_ball_owner < 0) && (s->blue_ball_owner < 0) ){
   if( (t =  sstate_time_to_blue_get_ball( s, &closest_red )) <
       sstate_time_to_red_get_ball( s, &closest_blue ) && ( t > 0 ) ){
          DEBUG("red get ball\n");  
        s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );
        s->ball_vel = v2_make(0,0);
        s->blue[closest_blue] = s->ball;
        s->blue_ball_owner = closest_blue;  
   }
 }
} 


void sstate_blue_receive_ball( SoccerState *s, int recv )
{
 float t, time_to_red_block, time_to_receive;
 int closest_red;

 if( (s->red_ball_owner < 0) && (s->blue_ball_owner < 0) ){ 
   time_to_red_block = sstate_time_to_blue_get_ball( s, &closest_red ); 
   time_to_receive = time_to_intersect( s->ball, s->ball_vel, s->blue[recv], 
                                        v2_norm( s->ball_vel ), s->blue_speed );

   if( (time_to_receive > 0 ) && (time_to_red_block > time_to_receive) ){
       t = DRAND()*( time_to_red_block - time_to_receive ) + time_to_receive;
       s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );  
       s->ball_vel = v2_make(0,0);
       s->blue[recv] = s->ball;
       s->blue_ball_owner = recv;  
   }
 }
}


SoccerAction sstate_red_kick_to_goal( SoccerState *s )
{
 float k;
 Vector2 p;
 SoccerAction action = saction_make(s->red_ball_owner);

 if( (s->red_ball_owner >= 0) && (s->blue_ball_owner < 0) ){
   for( k = -.5*s->goal_size; k < .5*s->goal_size; k += s->robot_radius ){ 
	 if(s->left_red_side)
       p =  v2_make( +s->field_w, k );
	 else
	   p =  v2_make( -s->field_w, k );
     is_red_kick_scored(s, p );
     if( s->goal_scored ){
       action.has_kicked = TRUE;
       action.ball_owner = -1;
	   action.kick_point = p;
       DEBUG( "goal scored!!!\n" );
       return action;
     }
   }
 }
 return action;
}  


static void is_red_kick_scored( SoccerState *s, Vector2 goal )
{
 int i;
 float dotprod;
 Vector2 goal_direction, enemy_relpos, proj;

 s->goal_scored = TRUE;

 for( i = 0; i < NPLAYERS; i++ ){
       goal_direction = v2_unit( v2_sub( goal, s->red[s->red_ball_owner] ) );
       enemy_relpos = v2_sub( s->blue[i], s->red[s->red_ball_owner] );
       proj = v2_scale( dotprod = v2_dot( enemy_relpos, goal_direction ), 
                      goal_direction );
       if( (dotprod > 0 ) && 
           (v2_norm( v2_sub( enemy_relpos, proj )) < s->robot_radius)  )
              s->goal_scored = FALSE;  /* if an opponent blocks it */
     }
}


void sstate_blue_kick_to_goal( SoccerState *s )
{
 int i;
 float k;
 Vector2 p;

 if( (s->blue_ball_owner >= 0) && (s->red_ball_owner < 0) ){
   for( k = -.5*s->goal_size; k < .5*s->goal_size; k += s->robot_radius ){ 
	 if(s->left_red_side)
       p =  v2_make( -s->field_w, k );
	 else
       p =  v2_make( +s->field_w, k );
     is_blue_kick_scored(s, p );
     if( s->goal_received )
       DEBUG( "goal received :(\n" );
   }
 }
}


static void is_blue_kick_scored( SoccerState *s, Vector2 goal )
{
 int i;
 float dotprod;
 Vector2  goal_direction, friend_relpos, proj;

 s->goal_received = TRUE;
 for( i = 0; i < NPLAYERS; i++ ){
       goal_direction = v2_unit( v2_sub( goal, s->blue[s->blue_ball_owner] ) );
       friend_relpos = v2_sub( s->red[i], s->blue[s->blue_ball_owner ] );
       proj = v2_scale( dotprod = v2_dot( friend_relpos, goal_direction ), 
                      goal_direction );
       if( (dotprod > 0 ) && 
          v2_norm( v2_sub( friend_relpos, proj )) < s->robot_radius )
          s->goal_received = FALSE;  /* if a friend robot blocks it */
     } 
}


SoccerAction sstate_red_pass( SoccerState *s, int recv, float recv_radius )
{
 double t;
 Vector2 p, new_recv_pos;
 SoccerAction action = saction_make(s->red_ball_owner);

 if( (s->red_ball_owner >= 0) && (s->red_ball_owner != recv ) &&
     (s->blue_ball_owner < 0) ){
        p = v2_make(recv_radius*DRAND()*cos(2*PI*DRAND()),
                    recv_radius*DRAND()*sin(2*PI*DRAND()) );
        new_recv_pos = v2_add( s->red[recv], p );
        if( (t = sstate_possible_red_pass( s, recv, new_recv_pos )) > 0 ){
           DEBUG2( "red pass: %i -> %i\n", s->red_ball_owner , recv );
           s->red_ball_owner = recv;
           s->red[recv] = new_recv_pos;
           s->ball = new_recv_pos;
          
           action.has_passed = TRUE; 
           action.ball_owner = recv;
           action.move[recv] = p;
		   action.kick_point = new_recv_pos;
        }
 } 
 return action;
}


void sstate_blue_pass( SoccerState *s, int recv, float recv_radius )
{
 double t;
 Vector2 p, new_recv_pos;

 if( (s->blue_ball_owner >= 0) && (s->blue_ball_owner != recv ) &&
     (s->red_ball_owner < 0 )){
         p = v2_make(recv_radius*DRAND()*cos(2*PI*DRAND()),
                     recv_radius*DRAND()*sin(2*PI*DRAND()) );
         new_recv_pos = v2_add( s->blue[recv], p );
         if( (t = sstate_possible_blue_pass( s, recv, new_recv_pos )) > 0 ){
             DEBUG2( "blue pass: %i -> %i\n", s->blue_ball_owner , recv );
             s->blue_ball_owner = recv;
             s->blue[recv] = new_recv_pos;
             s->ball = new_recv_pos;
         }
 } 
}


SoccerAction sstate_red_move( SoccerState *s, float radius )
{
 int i;
 float d;
 Vector2  p, new_pos;
 SoccerAction action = saction_make(s->red_ball_owner);

 
 for( i = 0; i < NPLAYERS; i++ ){
   do{
     p = v2_make(radius*DRAND()*cos(2*PI*DRAND()),
                 radius*DRAND()*sin(2*PI*DRAND()) );
     if( i == s->red_ball_owner )
        v2_scale( (s->red_dribble_speed/s->red_speed)*radius, p );
     d = v2_norm( p );
     new_pos = v2_add( s->red[i], p );
     action.move[i] = p; 
   }while( (sstate_min_blue_dist(s, new_pos) < d) ||
            !sstate_is_inside_field( s, new_pos ) );
   DEBUG5( "red move %i: (%f,%f) -> (%f,%f)", i,
            s->red[i].x, s->red[i].y , new_pos.x, new_pos.y );
   s->red[i] = new_pos;
   if( i == s->red_ball_owner ){
     s->ball = new_pos;
     DEBUG( "*");
   }
   DEBUG("\n");
 }
 return action;
}


void sstate_blue_move( SoccerState *s, float radius )
{
 int i;
 float d;
 Vector2 p, new_pos;

 for( i = 0; i < NPLAYERS; i++ ){
   do{
     p = v2_make(radius*DRAND()*cos(2*PI*DRAND()),
                 radius*DRAND()*sin(2*PI*DRAND()) );
     if( i == s->blue_ball_owner )
        v2_scale( (s->blue_dribble_speed/s->blue_speed)*radius, p );
     d = v2_norm( p );
     new_pos = v2_add( s->blue[i], p ); 
   }while( (sstate_min_red_dist(s, new_pos) < d) ||
           !sstate_is_inside_field( s, new_pos ) );
   DEBUG5( "blue move %i: (%f,%f) -> (%f,%f)", i,
           s->blue[i].x, s->blue[i].y , new_pos.x, new_pos.y ); 
   s->blue[i] = new_pos;
   if( i == s->blue_ball_owner ){
      s->ball = new_pos;
      DEBUG( "*");
   }
   DEBUG("\n");
 }
}

