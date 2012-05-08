#include "soccer.h"


SoccerAction saction_red_make( SoccerState *s )
{
 int i;
 SoccerAction action, *aux;
 
 aux = minimax_get_best_red_action();
 action = *aux;
 action.prune = TRUE;
 action.has_kicked = FALSE; 
 action.has_passed = FALSE;
 action.ball_owner = s->red_ball_owner;
 action.passer = s->red_passer;
 return action;
}


SoccerAction saction_blue_make( SoccerState *s )
{
 int i;
 SoccerAction action, *aux;

 aux = minimax_get_best_blue_action();
 action = *aux;
 action.prune = TRUE;
 action.has_kicked = FALSE; 
 action.has_passed = FALSE;
 action.ball_owner = s->blue_ball_owner;
 action.passer = s->blue_passer;
 return action;
}


void saction_red_act( SoccerState *s, SoccerAction *sa )
{
 int i, owner;

 if( sa->has_kicked ){
// sstate_restart_game_pos(s);
   return; 
 }

 for( i = 0; i < NPLAYERS; i++ )
   s->red[i] = sa->move[i];

 if( (owner = sa->ball_owner) >= 0 ){
     s->ball = s->red[owner]; 
     s->red_ball_owner = owner;
 } 

 s->red_passer = sa->passer;
}


void saction_blue_act( SoccerState *s, SoccerAction *sa )
{
 int i, owner;

 if( sa->has_kicked ){
// sstate_restart_game_pos(s);
  return; 
 }

 for( i = 0; i < NPLAYERS; i++ )
   s->blue[i] = sa->move[i];

 if( (owner = sa->ball_owner) >= 0 ){
     s->ball = s->blue[owner]; 
     s->blue_ball_owner = owner;
 }

 s->blue_passer = sa->passer;
}


void saction_simulate(  SoccerState *s, SoccerAction *red_act,
                         SoccerAction *blue_act, float dt )
{

 saction_simulate_red(s,red_act,dt);
 saction_simulate_blue(s,blue_act,dt);

 printf( "%i %i %i %i %i\n",  s->red_ball_owner, s->blue_ball_owner, red_act->ball_owner,
                             blue_act->ball_owner, s->blue_passer      );
 
 if( s->red_ball_owner >= 0 )
   s->ball = s->red[s->red_ball_owner];

 if( s->blue_ball_owner >= 0 )
   s->ball = s->blue[s->blue_ball_owner];

 if( ( s->red_ball_owner <  0 ) && ( s->blue_ball_owner < 0 ) &&
     ( v2_norm( s->ball_vel ) > EPS ) )
        s->ball = v2_add( s->ball, v2_scale( dt, s->ball_vel ) );
}


void saction_simulate_red( SoccerState *s, SoccerAction *red_act, 
                           float dt )
{
 int i; 

// static int last_passer = -1;
/*
 if( (last_passer != -1)  && 
     v2_norm( v2_sub( s->red[last_passer], s->ball ) ) > 1. )
       last_passer = -1;
*/

 if( (s->red_ball_owner < 0 ) && (s->blue_ball_owner < 0 ) &&
     (sstate_min_red_dist(s,s->ball) < sstate_min_blue_dist(s,s->ball)) &&
     (sstate_min_red_dist(s,s->ball) < .1 )// &&// &&
    // !red_act->has_passed 
     /*(sstate_closest_red(s, s->ball) != last_passer )*/ ){
         s->ball_vel = v2_make(0,0);
         s->ball = s->red[ sstate_closest_red(s, s->ball) ];
         s->red_ball_owner = sstate_closest_red(s, s->ball);
  //       last_passer = -1;
 }

 if( red_act->has_kicked ){
     printf( "Red Goal Scored!! goal covering = %f \n", 
              red_act->enemy_goal_covering );
 //    sstate_restart_game_pos(s);
 //    last_passer = -1;
     return;
 }

 if( red_act->has_passed ){
 //  last_passer = s->red_ball_owner;
   s->red_ball_owner = -1;
   s->ball_vel = v2_scale ( soccer_env()->red_pass_speed,
                 v2_unit( v2_sub( red_act->kick_point, s->ball )) );
 }
 
 for( i = 0; i < NPLAYERS; i++ ){
   if( v2_norm( v2_sub( red_act->move[i], s->red[i] )) > EPS ){ 
     float robot_speed;

     if( s->red_ball_owner == i )
        robot_speed = soccer_env()->red_dribble_speed;
     else 
        robot_speed = soccer_env()->red_speed;
 
     if( v2_norm( v2_sub( red_act->move[i], s->red[i] )) > .02 )            /* controller tol */
       s->red[i] = v2_add( s->red[i], v2_scale( dt*robot_speed,
                   v2_unit( v2_sub( red_act->move[i], s->red[i] ))) );

   } 
 }
}



void saction_simulate_blue( SoccerState *s, SoccerAction *blue_act, 
                            float dt )
{
 int i;
/* static int  last_passer = -1;

 if( (last_passer != -1)  && 
     v2_norm( v2_sub( s->blue[last_passer], s->ball ) ) > 1. )
       last_passer = -1; */

 if( (s->red_ball_owner < 0 ) && (s->blue_ball_owner < 0 ) &&
     (sstate_min_blue_dist(s,s->ball) < sstate_min_red_dist(s,s->ball)) &&
     (sstate_min_blue_dist(s,s->ball) < .1) //&&
    //  !blue_act->has_passed 
      //&&
     /*(sstate_closest_blue(s, s->ball) != last_passer )*/ ){
         s->ball_vel = v2_make(0,0);
         s->ball = s->blue[ sstate_closest_blue(s, s->ball) ];
         s->blue_ball_owner = sstate_closest_blue(s, s->ball);
      //   printf( "a %i \n", s->blue_ball_owner );
     //    last_passer = -1;
 }

 if( blue_act->has_kicked  ){
   printf( "Blue Goal Scored!! goal covering = %f \n", 
            blue_act->enemy_goal_covering );
 //  sstate_restart_game_pos(s);
 //  last_passer = -1;
   return; 
 }

 if( blue_act->has_passed ){
 //  last_passer = s->blue_ball_owner;
   s->blue_ball_owner = -1;
   s->ball_vel = v2_scale ( soccer_env()->blue_pass_speed,
                 v2_unit( v2_sub( blue_act->kick_point, s->ball )) );
 }
 
 for( i = 0; i < NPLAYERS; i++ ){
   if( v2_norm( v2_sub( blue_act->move[i], s->blue[i] )) > EPS ){ 
     float robot_speed;

     if( s->blue_ball_owner == i )
        robot_speed = soccer_env()->blue_dribble_speed;
     else 
        robot_speed = soccer_env()->blue_speed;
 
     if( v2_norm(v2_sub( blue_act->move[i], s->blue[i] )) > .02 )    
       s->blue[i] = v2_add( s->blue[i], v2_scale( dt*robot_speed,
                    v2_unit( v2_sub( blue_act->move[i], s->blue[i] ))) );

   } 
 }
}


