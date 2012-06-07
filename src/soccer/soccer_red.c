#include "soccer.h"


static void is_red_kick_scored( SoccerState *s, Vector2 goal );
static SoccerAction use_red_move_table( SoccerState *s, int robot );


SoccerAction sstate_red_get_ball( SoccerState *s )
{
 float t;
 int closest_red, closest_blue;
 SoccerAction action = saction_red_make(s);

 if( (s->red_ball_owner < 0) && (s->blue_ball_owner < 0) ){
    if( ((t =  sstate_time_to_red_get_ball( s, &closest_red )) <
        sstate_time_to_blue_get_ball( s, &closest_blue )) && ( t > 0 ) ){
        DEBUG("red get ball\n");  
        s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );
        s->ball_vel = v2_make(0,0);
        action.move[closest_red] = s->ball;
        s->red[closest_red] = s->ball;
        s->red_ball_owner = closest_red;  
        action.ball_owner = closest_red;
        action.prune = FALSE;
		action.type = get_ball;
    }
 }
 return action;
}


SoccerAction sstate_red_receive_ball( SoccerState *s, int recv )
{
 int maxIter = 10;
 int closest_blue, attempt;
 float t, time_to_blue_block, time_to_receive;
 SoccerAction action = saction_red_make(s);

 if( (s->red_ball_owner >= 0) || (s->blue_ball_owner >= 0) ) 
        return action; 
 
 time_to_blue_block = sstate_time_to_blue_get_ball( s, &closest_blue ); 
 time_to_receive = time_to_intersect( s->ball, s->ball_vel, s->red[recv], 
                                      v2_norm( s->ball_vel ), 
                                      soccer_env()->red_speed );

 if( (time_to_receive > 0 ) && (time_to_blue_block > time_to_receive) ){
   attempt = 0;
   do{
     attempt++;
     t = DRAND()*( time_to_blue_block - time_to_receive ) + time_to_receive;
   }
   while( (attempt < maxIter) && !sstate_is_valid_red_pos( s, recv,  
             v2_add( s->ball, v2_scale( t, s->ball_vel ) ) ) );
   if( attempt == maxIter )
     return action;
 
   s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );  
   s->ball_vel = v2_make(0,0);
   action.move[recv] = s->ball;
   s->red[recv] = s->ball;
   s->red_ball_owner = recv;  
   action.ball_owner = recv;
   action.prune = FALSE;
   action.type = receive_ball;
 }
 return action;
}
  

SoccerAction sstate_red_kick_to_goal( SoccerState *s )
{
 float k;
 Vector2 p;
 SoccerAction action = saction_red_make(s);
 Vector2 blue_goal;

 if(soccer_env()->left_red_side)
   blue_goal = v2_make( +soccer_env()->hfield_w, 0 );
 else
   blue_goal = v2_make( -soccer_env()->hfield_w, 0 );

 if( (s->red_ball_owner >= 0) && (s->blue_ball_owner < 0) &&
      (v2_norm( v2_sub( s->ball, blue_goal)) <
       soccer_env()->max_red_kick_dist) ){
   s->blue_goal_covering = 1;
   for( k = -.5*soccer_env()->goal_size; k < .5*soccer_env()->goal_size;
        k += soccer_env()->robot_radius ){ 
          p = blue_goal;
		  p.y += k;
          is_red_kick_scored(s, p );
          if( s->goal_scored ){
              s->blue_goal_covering -= (soccer_env()->robot_radius/
                                        soccer_env()->goal_size);
			  action.type = kick_to_goal;
			  action.kick_point = p;
              action.enemy_goal_covering = s->blue_goal_covering;
              action.ball_owner = -1;
              DEBUG( "goal scored!!!\n" );
              action.prune = FALSE;
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
           (v2_norm( v2_sub( enemy_relpos, proj )) < soccer_env()->robot_radius)  )
              s->goal_scored = FALSE;  /* if an opponent blocks it */
     }
}


SoccerAction sstate_red_pass( SoccerState *s, int recv, float recv_radius )
{
 double t;
 Vector2 p, new_recv_pos;
 SoccerAction action = saction_red_make(s);

 if( (s->red_ball_owner >= 0) && (s->red_ball_owner != recv ) &&
     (s->blue_ball_owner < 0) ){
        p = v2_make(recv_radius*DRAND()*cos(2*PI*DRAND()),
                    recv_radius*DRAND()*sin(2*PI*DRAND()) );
        new_recv_pos = v2_add( s->red[recv], p );
        if( sstate_is_valid_red_pos( s, recv, new_recv_pos) &&
            ((t = sstate_possible_red_pass( s, recv, new_recv_pos )) > 0) ){
           DEBUG2( "red pass: %i -> %i\n", s->red_ball_owner , recv );
           s->red_passer = s->red_ball_owner;
           s->red_ball_owner = recv;
           s->red[recv] = new_recv_pos;
           s->ball = new_recv_pos;

		   action.type = pass;
           action.passer = s->red_passer;
           action.passer_pos = s->red[ action.passer ];
           action.kick_point = new_recv_pos;
           action.ball_owner = recv;
           action.move[recv] = new_recv_pos;
           action.prune = FALSE;
        }
 } 
 return action;
}



SoccerAction sstate_red_move( SoccerState *s, int robot, float radius )
{
 int maxIter = 10;
 int attempt = 0;
 float d;
 Vector2  p, new_pos;
 SoccerAction action;

 action = use_red_move_table(s, robot);    
 do{
	 attempt++; 
     p = v2_make(radius*DRAND()*cos(2*PI*DRAND()),
                 radius*DRAND()*sin(2*PI*DRAND()) );
     if( robot == s->red_ball_owner )
        v2_scale( (soccer_env()->red_dribble_speed/
                   soccer_env()->red_speed)*radius, p );
     d = v2_norm( p );
     new_pos = v2_add( s->red[robot], p );
     action.move[robot] = new_pos; 
     action.prune = FALSE;
   }while( (attempt < maxIter) && ((sstate_min_blue_dist(s, new_pos) < d ) ||
           (!sstate_is_valid_red_pos( s, robot, new_pos ) &&
             sstate_is_inside_field( s, s->red[robot] ))) );
   if( attempt == maxIter )
		return action;
   DEBUG5( "red move %i: (%f,%f) -> (%f,%f)", robot,
            s->red[robot].x, s->red[robot].y , new_pos.x, new_pos.y );
   s->red[robot] = new_pos;
   if( robot == s->red_ball_owner ){
     s->ball = new_pos;
     DEBUG( "*");
   }
   DEBUG("\n");
   action.type = move;
 
 return action;
}


static SoccerAction use_red_move_table( SoccerState *s, int robot )
{
 int i;
 Vector2 disp, p;
 SoccerAction action = saction_red_make(s);

 for( i=0; i < NPLAYERS; i++ )
   if( i != robot ){
     disp = v2_sub( get_red_move_table(i), s->red[i] );
     if( v2_norm( disp ) > .5*soccer_env()->robot_radius ){
		  p = get_red_move_table(i);
       // p = v2_add( s->red[i], 
       //             v2_scale( /*100**/(1./(NPLAYERS-1))*soccer_env()->sample_period*
       //                       soccer_env()->red_speed, v2_unit(disp) ));
        if( sstate_is_valid_red_pos( s, i, p ) )
          s->red[i] = p;
     }
     action.move[i] = s->red[i];
   } 

 return action;
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


float sstate_time_to_red_get_ball( SoccerState *s, int *closest )
{
 int i;
 float t, mint;

 mint = MAX_FLOAT;
 for( i = 0; i < NPLAYERS; i++ ){ 
   t = time_to_intersect( s->ball, s->ball_vel, s->red[i], 
                          v2_norm( s->ball_vel ), soccer_env()->red_speed );
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

 t_move = v2_norm(v2_sub( s->red[recv], p ))/soccer_env()->red_speed;
 t_pass = v2_norm(v2_sub( s->red[ s->red_ball_owner ], p ))/
                          soccer_env()->red_pass_speed;

 for( i = 0; i < NPLAYERS; i++ ){
    t1 = time_to_intersect( s->ball, pass_dir, s->blue[i], 
                            soccer_env()->red_pass_speed, soccer_env()->blue_speed );
    t2 = time_to_intersect( s->red[recv], move_dir, s->blue[i], 
                            soccer_env()->red_speed, soccer_env()->blue_speed );
    if( (( t1 > 0 ) && (t1 < t_pass)) ||  (( t2 > 0 ) && (t2 < t_move )))
       return -1;
 }

 if( t_move < t_pass )
    return t_pass;
 else
    return -1; 
}
