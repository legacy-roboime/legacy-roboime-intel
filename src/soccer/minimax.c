#include "minimax.h"

static Boolean max_is_root = FALSE;

static SoccerAction best_red_action;
static SoccerAction prev_best_red_action;
static SoccerAction best_blue_action;
static SoccerAction prev_best_blue_action;

static int red_robot = 0;
static int blue_robot = 0;

void minimax_init( SoccerState *s )
{
 int i;
 best_red_action.prune = TRUE;
 best_blue_action.prune = TRUE; 

 for( i = 0; i < NPLAYERS; i++ ){
   best_red_action.move[i] = s->red[i];
   best_blue_action.move[i] = s->blue[i];
   prev_best_red_action.move[i] = s->red[i];
   prev_best_blue_action.move[i] = s->blue[i];
 }
 best_red_action.ball_owner = s->red_ball_owner;
 best_blue_action.ball_owner = s->blue_ball_owner;
}


void minimax_use_next_red_robot(void)
{
 red_robot = (red_robot+1)%6;
}


void minimax_use_next_blue_robot(void)
{
 blue_robot = (blue_robot+1)%6;
}

SoccerAction* minimax_get_best_red_action( void )
{
 prev_best_red_action = best_red_action;
 prev_best_red_action.prune = FALSE;
 return &best_red_action;       
}


SoccerAction* minimax_get_best_blue_action( void )
{
 prev_best_blue_action = best_blue_action;
 prev_best_blue_action.prune = FALSE;
 return &best_blue_action;       
}


void  minimax_play( SoccerState *s, int depth )
{
 prev_best_red_action = saction_red_make(s);
 prev_best_blue_action = saction_blue_make(s);
 if(  sstate_min_red_dist(s, s->ball ) < 
      sstate_min_blue_dist(s, s->ball ))
   minimax_playMax( s, depth );
 else
   minimax_playMin( s, depth );
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
 int i, attempt;
 float aux; 
 SoccerState saux;
 SoccerAction action;

 if( depth == 0 ){
      saux = s;
      sstate_red_kick_to_goal(&saux); 
      return sstate_evaluate(&saux);
 }
 for( i = 0; i < MAX_NPLAYS; i++ ){
     saux = s;
     attempt = 0;
     do{
       attempt++;
       action = minimax_expandMax( &saux, i, depth );
     }
     while( (saction_red_elapsed_time( &action ) > parent_duration) && (attempt < 10) );
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
 int i, attempt;
 float aux;
 SoccerState saux;
 SoccerAction action;

 if( depth == 0 ){
     saux = s;
     sstate_blue_kick_to_goal(&saux); 
     return sstate_evaluate(&saux);
 }
 for( i = 0; i < MIN_NPLAYS; i++ ){
      saux = s;
      attempt = 0;
      do{
         attempt++;
         action = minimax_expandMin( &saux, i, depth );
      }
      while( (saction_blue_elapsed_time( &action ) > parent_duration) && (attempt < 10) );
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


SoccerAction minimax_expandMax( SoccerState *s, int i, int depth )
{
 float move_radius , recv_radius;
 SoccerAction action = saction_red_make(s);
 move_radius = soccer_env()->red_move_radius;
 recv_radius = soccer_env()->red_recv_radius; 

 if( s->red_ball_owner >= 0 ){
   switch( i ){
     case 0: action = sstate_red_kick_to_goal(s); break;
     case 2: action = sstate_red_pass(s,0, recv_radius ); break;
     case 3: action = sstate_red_pass(s,1, recv_radius ); break;
     case 4: action = sstate_red_pass(s,2, recv_radius ); break;
     case 5: action = sstate_red_pass(s,3, recv_radius ); break;
     case 6: action = sstate_red_pass(s,4, recv_radius ); break; 
     case 7: action = sstate_red_pass(s,5, recv_radius ); break;   
   }
 }
 else{
   switch( i ){ 
    case 1: action = sstate_red_get_ball(s); break;
    case 8: action = sstate_red_receive_ball(s,0); break;
    case 9: action = sstate_red_receive_ball(s,1); break;
    case 10: action = sstate_red_receive_ball(s,2); break;
    case 11: action = sstate_red_receive_ball(s,3); break;
    case 12: action = sstate_red_receive_ball(s,4); break;
    case 13: action = sstate_red_receive_ball(s,5); break;   
   }
 }

 
 if( (i > 13) && (i < 98 ) )
     action = sstate_red_move(s,  &prev_best_red_action, red_robot, move_radius );
/* if( (i>= 50) && ( i < 60) )
     action = sstate_red_move(s,  &prev_best_red_action, red_robot,(1./2)*move_radius );
 if( (i >= 60) && ( i < 70) )
     action = sstate_red_move(s,  &prev_best_red_action, red_robot,(1./4)*move_radius );
if( (i >= 70) && ( i < 80) )
     action = sstate_red_move(s,  &prev_best_red_action, red_robot, (1./8)*move_radius);
if( (i >= 80) && ( i < 90) )
     action = sstate_red_move(s,  &prev_best_red_action, red_robot, (1./16)*move_radius );
if( (i >= 90) && ( i < 98) )
     action = sstate_red_move(s,  &prev_best_red_action, red_robot, (1./32)*move_radius  ); */
 if( i == 99 ){
     saction_red_act( s, &prev_best_red_action );
     action = prev_best_red_action;
 } 
 if( i == 100 )
   action.prune = FALSE; 
 
 return action;
}


SoccerAction minimax_expandMin( SoccerState *s, int i, int depth )
{
 float move_radius , recv_radius;
 SoccerAction action = saction_blue_make(s);
 move_radius = soccer_env()->blue_move_radius;
 recv_radius = soccer_env()->blue_recv_radius; 

 if( s->blue_ball_owner >= 0 ){
   switch( i ){
     case 0: action = sstate_blue_kick_to_goal(s); break;
     case 2: action = sstate_blue_pass(s,0, recv_radius ); break;
     case 3: action = sstate_blue_pass(s,1, recv_radius ); break;
     case 4: action = sstate_blue_pass(s,2, recv_radius ); break;
     case 5: action = sstate_blue_pass(s,3, recv_radius ); break;
     case 6: action = sstate_blue_pass(s,4, recv_radius ); break; 
     case 7: action = sstate_blue_pass(s,5, recv_radius ); break;  
   }
 }
 else{
   switch( i ){ 
    case 1: action = sstate_blue_get_ball(s); break;
    case 8: action = sstate_blue_receive_ball(s,0); break;
    case 9: action = sstate_blue_receive_ball(s,1); break;
    case 10: action = sstate_blue_receive_ball(s,2); break;
    case 11: action = sstate_blue_receive_ball(s,3); break;
    case 12: action = sstate_blue_receive_ball(s,4); break;
    case 13: action = sstate_blue_receive_ball(s,5); break; 
   }
 }

 if( (i > 13) && (i < 98 ) )
     action = sstate_blue_move(s,  &prev_best_blue_action, blue_robot, move_radius ); /*
 if( (i>= 50) && ( i < 60) )
     action = sstate_blue_move(s,  &prev_best_blue_action, blue_robot,(1./2)*move_radius );
 if( (i >= 60) && ( i < 70) )
     action = sstate_blue_move(s,  &prev_best_blue_action, blue_robot,(1./4)*move_radius );
if( (i >= 70) && ( i < 80) )
     action = sstate_blue_move(s,  &prev_best_blue_action, blue_robot, (1./8)*move_radius);
if( (i >= 80) && ( i < 90) )
     action = sstate_blue_move(s,  &prev_best_blue_action, blue_robot, (1./16)*move_radius );
if( (i >= 90) && ( i < 98) )
     action = sstate_blue_move(s,  &prev_best_blue_action, blue_robot, (1./32)*move_radius  ); */
 if( i == 99 ){
     saction_blue_act( s, &prev_best_blue_action );
     action = prev_best_blue_action;
 } 
 if( i == 100 )
   action.prune = FALSE; 
 
 return action;
}


