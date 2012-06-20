#include "soccer.h"

static void is_blue_kick_scored( SoccerState *s, Vector2 goal );
static SoccerAction use_blue_move_table( SoccerState *s, int robot );


SoccerAction sstate_blue_get_ball( SoccerState *s )
{
 float t;
 int closest_red, closest_blue;
 SoccerAction action = saction_blue_make(s);

 if( (s->red_ball_owner < 0) && (s->blue_ball_owner < 0) ){
   if( ((t =  sstate_time_to_blue_get_ball( s, &closest_blue )) <
       sstate_time_to_red_get_ball( s, &closest_red )) && ( t > 0 ) ){
          DEBUG("blue get ball\n");  
        s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );
        s->ball_vel = v2_make(0,0);
        action.move[closest_blue] = s->ball;
        s->blue[closest_blue] = s->ball;
        s->blue_ball_owner = closest_blue;  
        action.ball_owner = closest_blue;
        action.prune = FALSE;
		action.type = get_ball;
   }
 }
 return action;
} 


SoccerAction sstate_blue_receive_ball( SoccerState *s, int recv )
{
 int closest_red, attempt;
 int maxIter = 10;
 float t, time_to_red_block, time_to_receive;
 SoccerAction action = saction_blue_make(s);

 if( (s->red_ball_owner >= 0) || (s->blue_ball_owner >= 0))
     return action;

 time_to_red_block = sstate_time_to_blue_get_ball( s, &closest_red ); 
 time_to_receive = time_to_intersect( s->ball, s->ball_vel, s->blue[recv], 
                                      v2_norm( s->ball_vel ), 
                                      soccer_env()->blue_speed );

 if( (time_to_receive > 0 ) && (time_to_red_block > time_to_receive) ){
       attempt = 0;
       do{
         attempt++; 
         t = DRAND()*( time_to_red_block - time_to_receive ) + time_to_receive;
       }
       while( (attempt < maxIter) && !sstate_is_valid_blue_pos( s, recv, 
                 v2_add( s->ball, v2_scale( t, s->ball_vel ) ) ) ); 
       if( attempt == maxIter )
         return action;

       s->ball = v2_add( s->ball, v2_scale( t, s->ball_vel ) );  
       s->ball_vel = v2_make(0,0);
       action.move[recv] = s->ball;
       s->blue[recv] = s->ball;
       s->blue_ball_owner = recv; 
       action.ball_owner = recv; 
       action.prune = FALSE; 
	   action.type = receive_ball;
  }
 return action;
}


SoccerAction sstate_blue_kick_to_goal( SoccerState *s )
{
 int i;
 float dy = .15;
 float k;
 Vector2 p;
 SoccerAction action = saction_blue_make(s);

 if( (s->blue_ball_owner >= 0) && (s->red_ball_owner < 0) &&
     (v2_norm( v2_sub( s->ball, soccer_env()->red_goal )) <
       soccer_env()->max_blue_kick_dist) ){
   s->red_goal_covering = 1;
   for( k = -.5*soccer_env()->goal_size; k < .5*soccer_env()->goal_size;
        k += .2 ){ 
	  p = soccer_env()->red_goal;
	  p.y += k;
          is_blue_kick_scored(s, p ); 
          if( s->goal_received ){
              s->red_goal_covering -= ( soccer_env()->robot_radius/
                                        soccer_env()->goal_size );
			  action.type = kick_to_goal;
			  action.kick_point = p;
              action.enemy_goal_covering = s->red_goal_covering;
              action.ball_owner = -1;  
              DEBUG( "goal received :(\n" );
              action.prune = FALSE;
          }
   }
 }
 return action;
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
          v2_norm( v2_sub( friend_relpos, proj )) < soccer_env()->robot_radius )
          s->goal_received = FALSE;  /* if a friend robot blocks it */
     } 
}


SoccerAction sstate_blue_pass( SoccerState *s, int recv, float recv_radius )
{
 double t;
 Vector2 p, new_recv_pos;
 SoccerAction action = saction_blue_make(s);

 if( (s->blue_ball_owner >= 0) && (s->blue_ball_owner != recv ) &&
     (s->red_ball_owner < 0 )){
         p = v2_make(recv_radius*DRAND()*cos(2*PI*DRAND()),
                     recv_radius*DRAND()*sin(2*PI*DRAND()) );
         new_recv_pos = v2_add( s->blue[recv], p );
         if( sstate_is_valid_blue_pos( s, recv, new_recv_pos) &&
             ((t = sstate_possible_blue_pass( s, recv, new_recv_pos )) > 0) ){
             DEBUG2( "blue pass: %i -> %i\n", s->blue_ball_owner , recv );
             s->blue_passer = s->blue_ball_owner;
             s->blue_ball_owner = recv;
             s->blue[recv] = new_recv_pos;
             s->ball = new_recv_pos;

			 action.type = pass;
             action.passer = s->blue_passer;
             action.passer_pos = s->blue[ action.passer ];
             action.kick_point = new_recv_pos;
             action.ball_owner = recv;
             action.move[recv] = new_recv_pos;
             action.prune = FALSE;
         }
 } 
 return action;
}


SoccerAction sstate_blue_block( SoccerState *s, int robot, Vector2 src_point )
{
 int maxIter = 10;	 
 int attempt = 0;
 Vector2 goal_point, new_pos;
 SoccerAction action = use_blue_move_table(s, robot);
 
 do{
   attempt++;
   goal_point = v2_add( soccer_env()->blue_goal,
	                    v2_make(0, (DRAND() - .5)*soccer_env()->goal_size ));
   new_pos = v2_lerp( DRAND(), goal_point, src_point );
 }
 while( (attempt < maxIter) && (!sstate_is_valid_blue_pos( s, robot, new_pos )) ); 

 if( attempt ==  maxIter )
	return action;
 
 action.type = move;
 action.move[robot] = new_pos;
 s->blue[robot] = new_pos;
 if( robot == s->blue_ball_owner )
    s->ball = new_pos;
 action.prune = FALSE;
 return action;
}


SoccerAction sstate_blue_move( SoccerState *s, int robot, float radius )
{
 int attempt = 0;
 int maxIter = 10;
 float d;
 Vector2 p, new_pos;
 SoccerAction action = use_blue_move_table(s, robot); 

 action = use_blue_move_table(s, robot);  
 do{
     attempt++; 
     p = v2_make(radius*DRAND()*cos(2*PI*DRAND()),
                 radius*DRAND()*sin(2*PI*DRAND()) );
     if( robot == s->blue_ball_owner )
        v2_scale( (soccer_env()->blue_dribble_speed/
                   soccer_env()->blue_speed)*radius, p );
     d = v2_norm( p );
     new_pos = v2_add( s->blue[robot], p ); 
     action.move[robot] = new_pos; 
     action.prune = FALSE;
 }while( (attempt < maxIter) && (/*(sstate_min_red_dist(s, new_pos) < d )  ||*/
           (!sstate_is_valid_blue_pos( s, robot, new_pos ) &&
             sstate_is_inside_field( s, s->blue[robot] ))) );
 if( attempt == maxIter )
	 return action;
 DEBUG5( "blue move %i: (%f,%f) -> (%f,%f)", i,
          s->blue[robot].x, s->blue[robot].y , new_pos.x, new_pos.y ); 
 s->blue[robot] = new_pos;
 if( robot == s->blue_ball_owner ){
     s->ball = new_pos;
     DEBUG( "*");
 }
 DEBUG("\n");
 action.type = move;
 
 return action;
}



static SoccerAction use_blue_move_table( SoccerState *s, int robot )
{
 int i;
 Vector2 disp, p;
 SoccerAction action = saction_blue_make(s);

 for( i=0; i < NPLAYERS; i++ )
   if( i != robot ){
     disp = v2_sub( get_blue_move_table(i), s->blue[i] );
     if( v2_norm( disp ) > .1*soccer_env()->robot_radius ){
         p =  v2_add( s->blue[i], 
                     v2_scale( MAX(1., NPLAYERS*soccer_env()->sample_period
                                       *soccer_env()->blue_speed ),
                     v2_unit(disp) ));
        if( sstate_is_valid_blue_pos( s, i, p ) )
          s->blue[i] = p;
     }
     action.move[i] = s->blue[i];
   } 

 return action;
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


float sstate_time_to_blue_get_ball( SoccerState *s, int *closest )
{
 int i;
 float t, mint;

 mint = MAX_FLOAT;
 for( i = 0; i < NPLAYERS; i++ ){ 
   t = time_to_intersect( s->ball, s->ball_vel, s->blue[i],
                          v2_norm( s->ball_vel ), soccer_env()->blue_speed );
   if( t < mint ){
      mint = t;
      if( closest != NULL )
         *closest = i;
   } 
 }
 return mint;
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
 t_move = v2_norm(v2_sub( s->blue[recv], p ))/soccer_env()->blue_speed;
 t_pass = v2_norm(v2_sub( s->blue[ s->blue_ball_owner ], p ))/
                  soccer_env()->blue_pass_speed;

 for( i = 0; i < NPLAYERS; i++ ){
    t1 = time_to_intersect( s->ball, pass_dir, s->red[i], 
                            soccer_env()->blue_pass_speed, soccer_env()->red_speed );
    t2 = time_to_intersect( s->blue[recv], move_dir, s->red[i],
                            soccer_env()->blue_speed, soccer_env()->red_speed );
    if( (( t1 > 0 ) && (t1 < t_pass)) ||  (( t2 > 0 ) && (t2 < t_move )))
       return -1;
 }

 if( t_move < t_pass )
    return t_pass;
 else
    return -1; 
}

