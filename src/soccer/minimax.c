#include "minimax.h"

static Boolean max_is_root = FALSE;

static SoccerAction best_red_action;
static SoccerAction best_blue_action;

static int red_robot = 0;
static int blue_robot = 0;

static int current_index = 0;
static int red_robots_list[NPLAYERS];
static int blue_robots_list[NPLAYERS];

static void robots_lists_generate( SoccerState *s );
static Boolean not_in_list( int tested, int* robot_list, int end ); 

static void adjust_move_tables( void );


void minimax_init( SoccerState *s )
{
 int i;
 best_red_action.prune = TRUE;
 best_blue_action.prune = TRUE;
 best_red_action.type = null_action;
 best_blue_action.type = null_action;

 for( i = 0; i < NPLAYERS; i++ ){
   best_red_action.move[i] = s->red[i];
   best_blue_action.move[i] = s->blue[i];
   set_red_move_table( i, s->red[i] );
   set_blue_move_table( i, s->blue[i] );
 }

 best_red_action.ball_owner = s->red_ball_owner;
 best_blue_action.ball_owner = s->blue_ball_owner;
}


void minimax_use_next_red_robot(void)
{
 red_robot = red_robots_list[current_index];
 current_index = ( current_index + 1 )%NPLAYERS;
}


void minimax_use_next_blue_robot(void)
{
 blue_robot = blue_robots_list[current_index];
 current_index = ( current_index + 1 )%NPLAYERS;
}

SoccerAction* minimax_get_best_red_action( void )
{
 return &best_red_action;       
}


SoccerAction* minimax_get_best_blue_action( void )
{
 return &best_blue_action;       
}


void  minimax_play( SoccerState *s, int depth )
{
 s->red_time_stamp = 0;
 s->blue_time_stamp = 0; 

 if( current_index == 0 )
   robots_lists_generate(s);

 red_robot = red_robots_list[current_index];
 blue_robot = blue_robots_list[current_index];

 //printf( "%i %i %i\n", red_robot, blue_robot,  current_index  );

 current_index = ( current_index + 1 )%NPLAYERS;

// minimax_use_next_blue_robot(); 
 //red_robot = (int)(DRAND()*NPLAYERS);

 /*
 if(  sstate_min_red_dist(s, s->ball ) + .3 < 
      sstate_min_blue_dist(s, s->ball ) ){
        minimax_use_next_blue_robot(); 
        red_robot = sstate_closest_red(s, s->ball);
 }
 else{
   minimax_use_next_red_robot();	  
   blue_robot = sstate_closest_blue(s, s->ball);
 }
 */

 if(  sstate_min_red_dist(s, s->ball ) < 
      sstate_min_blue_dist(s, s->ball ))
   minimax_playMax( s, depth );
 else
   minimax_playMin( s, depth );

 adjust_move_tables();
}

void robots_lists_generate( SoccerState *s )
{
 int i, j, closest_red, closes_blue;
 float dist, smallest_red_dist, smallest_blue_dist;

 red_robots_list[0] = sstate_closest_red(s, s->ball);
 blue_robots_list[0] = sstate_closest_blue(s, s->ball);
 
 for( i = 1; i < NPLAYERS; i++ ){
	smallest_red_dist = MAX_FLOAT;
	smallest_blue_dist = MAX_FLOAT;  
    for( j = 0; j < NPLAYERS; j++ ){
       if( (( dist = v2_norm( v2_sub( s->red[j], s->red[i-1]) ) ) < smallest_red_dist) && not_in_list(  j, red_robots_list, i ) ){
          red_robots_list[i] = j;
		  smallest_red_dist = dist;
	   } 
       if( (( dist = v2_norm( v2_sub( s->blue[j], s->blue[i-1]) ) ) < smallest_blue_dist) && not_in_list( j, blue_robots_list, i )){
          blue_robots_list[i] = j;
		  smallest_blue_dist = dist;
	   } 
	}
 }
}

Boolean not_in_list( int tested, int* robot_list, int end )
{
 Boolean is_not_in_list = TRUE;
 int i;

 for( i = 0; i < end; i++ )
    if( tested == robot_list[i] )
      is_not_in_list = FALSE;
 
 return is_not_in_list;
}


void adjust_move_tables( void )
{
 int i;

 for( i=0; i < NPLAYERS; i++ ){
   if( v2_norm( v2_sub( best_red_action.move[i], 
                 best_red_action.pos[i] ) ) > EPS )
        set_red_move_table( i, best_red_action.move[i] );
   if( v2_norm( v2_sub( best_blue_action.move[i], 
                 best_blue_action.pos[i] ) ) > EPS )
        set_blue_move_table( i, best_blue_action.move[i] ); 
 }
}


void  minimax_playMax( SoccerState *s, int depth )
{
 max_is_root = TRUE; 
 minimax_getMaxValue( *s, MINIMAX_MAX_LEVEL, MAX_FLOAT, -MAX_FLOAT, MAX_FLOAT );
}


void  minimax_playMin( SoccerState *s, int depth )
{
 max_is_root = FALSE; 
 minimax_getMinValue( *s, MINIMAX_MAX_LEVEL, MAX_FLOAT, -MAX_FLOAT, MAX_FLOAT );
}


float minimax_getMaxValue(SoccerState s, int depth,
                          float parent_duration, float alpha, float beta )
{
 int i;
 float aux; 
 SoccerState saux;
 SoccerAction action;

 if( depth == 0 ){
      saux = s;
      //sstate_red_kick_to_goal(&saux);
      return sstate_evaluate(&saux)*minimax_red_time_weight_func(&s);
 }
 for( i = 0; i < MAX_NPLAYS; i++ ){
     saux = s;
     action = minimax_expandMax( &saux, i, depth );
     if( !action.prune ){
       if( alpha < (aux = minimax_getMinValue( saux, depth-1, 
                    saction_red_elapsed_time( &action ), alpha, beta )) ){
           alpha = aux;
           if( depth == ( max_is_root ? MINIMAX_MAX_LEVEL :
                          MINIMAX_MAX_LEVEL-1 ) )
             best_red_action = action;
       }
       if( beta <= alpha )
          break;
     }
 }
  return alpha;
}


float minimax_getMinValue(SoccerState s, int depth, 
                          float parent_duration, float alpha, float beta )
{
 int i;
 float aux;
 SoccerState saux;
 SoccerAction action;

 if( depth == 0 ){
     saux = s;
     //sstate_blue_kick_to_goal(&saux); 
     return sstate_evaluate(&saux)*minimax_blue_time_weight_func(&s);
 }
 for( i = 0; i < MIN_NPLAYS; i++ ){
      saux = s;
      action = minimax_expandMin( &saux, i, depth );
      if( !action.prune ){
          if( beta > (aux = minimax_getMaxValue( saux, depth-1, 
                      saction_blue_elapsed_time( &action ), alpha, beta )) ){
              beta = aux;
              if( depth == ( max_is_root ? MINIMAX_MAX_LEVEL - 1:
                             MINIMAX_MAX_LEVEL ))
                 best_blue_action = action;
          } 
          if( beta <= alpha )
             break;
      }  
  }
  return beta;
}


float minimax_red_time_weight_func( SoccerState *s )
{
 return 1 + .001*( -s->red_time_stamp + s->blue_time_stamp);
}

float minimax_blue_time_weight_func( SoccerState *s )
{
 return 1 + .001*( -s->red_time_stamp + s->blue_time_stamp);
}
 

SoccerAction minimax_expandMax( SoccerState *s, int i, int depth )
{
 int j;
 float move_radius , recv_radius;
 SoccerAction action = saction_red_make(s);
 move_radius = soccer_env()->red_move_radius;
 recv_radius = soccer_env()->red_recv_radius; 

 if( s->red_ball_owner >= 0 ){
     if( i== 0 )
        action = sstate_red_kick_to_goal(s);
     if ( (i >= 2) && (i < 8) )
       action = sstate_red_pass(s, i%NPLAYERS, recv_radius ); 
 }
 else{
     if( i == 1 )
       action = sstate_red_get_ball(s); 
     if( (i >=8 ) && (i < 14) )
       action = sstate_red_receive_ball( s, i%NPLAYERS ); 
 }
 if( (i == 14) )
	   action.prune = FALSE;
 if( (i >= 15) &&  (i < 25) )
	 action = sstate_red_move(s, blue_robot, .3 );  
 if( (i >= 25) &&  (i < 30) )
	 action = sstate_red_move(s, blue_robot, .8 );  
 if( (i >= 30) &&  (i < 35) )
	 action = sstate_red_move(s, blue_robot, 1.2 );  
 if( (i >= 35) && (i < (MAX_NPLAYS-2)) )
     action = sstate_red_move(s, red_robot, move_radius );
 if( i ==  (MAX_NPLAYS-2)  ){
	 //for( j=0; j < NPLAYERS;j++ ){
  //     s->red[j] = get_red_move_table(j);
  //     action.move[j] = get_red_move_table(j);
  //     action.prune = FALSE;
	 //}
	 s->red[red_robot] = get_red_move_table(red_robot);
     action.move[red_robot] = get_red_move_table(red_robot);
     action.prune = FALSE;
 }   
 if( i ==  (MAX_NPLAYS-1)  )
	action = sstate_red_block(s, red_robot, s->ball); 

 s->red_time_stamp += saction_red_elapsed_time(&action);  
 return action;
}


SoccerAction minimax_expandMin( SoccerState *s, int i, int depth )
{
 float move_radius , recv_radius;
 SoccerAction action = saction_blue_make(s);
 move_radius = soccer_env()->blue_move_radius;
 recv_radius = soccer_env()->blue_recv_radius; 
 
 if( s->blue_ball_owner >= 0 ){
     if( i== 0 )
        action = sstate_blue_kick_to_goal(s);
  //   if ( (i >= 2) && (i < 8) )
   //    action = sstate_blue_pass(s, i%NPLAYERS, recv_radius ); 
 }
 else{
     if( i == 1 )
       action = sstate_blue_get_ball(s); 
  //   if( (i >=8 ) && (i <14) )
   //    action = sstate_blue_receive_ball( s, i%NPLAYERS ); 
 }
  if( i == 14 )
	  action.prune = FALSE; 
 if( (i >= 15) &&  (i < 25) )
	 action = sstate_blue_move(s, blue_robot, .2 );  
 if( (i >= 25) &&  (i < 30) )
	 action = sstate_blue_move(s, blue_robot, .8 );  
 if( (i >= 30) &&  (i < 35) )
	 action = sstate_blue_move(s, blue_robot, 1.2 );  
 if( (i >= 35) &&  i < (MIN_NPLAYS - 2) )
     action = sstate_blue_move(s, blue_robot, move_radius ); 

 if( i == (MIN_NPLAYS - 2) ){
     s->blue[blue_robot] = get_blue_move_table(blue_robot);
     action.move[blue_robot] = get_blue_move_table(blue_robot);
     action.prune = FALSE;
 }   
 if( i == (MIN_NPLAYS - 1) )
	 sstate_blue_block(s, blue_robot, s->ball); 
 
 s->blue_time_stamp += saction_blue_elapsed_time(&action); 
 return action;
}

