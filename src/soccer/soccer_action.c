#include "soccer.h"


SoccerAction saction_red_make( SoccerState *s )
{
 int i;
 SoccerAction action;

 for( i = 0; i < NPLAYERS; i++ ){ 
    action.move[i] = s->red[i];
    action.pos[i] = s->red[i];
 }
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
 SoccerAction action; 

 for( i = 0; i < NPLAYERS; i++ ){ 
    action.move[i] = s->blue[i];
    action.pos[i] = s->blue[i];
 }
 action.prune = TRUE;
 action.has_kicked = FALSE; 
 action.has_passed = FALSE;
 action.ball_owner = s->blue_ball_owner;
 action.passer = s->blue_passer;
 return action;
}


float saction_red_elapsed_time( SoccerAction *sa )
{
 int i;
 float dist, move_delay = 0;
 float pass_delay = 0;

 for( i = 0; i < NPLAYERS; i++ ){
   dist = v2_norm(v2_sub( sa->pos[i], sa->move[i] ));
   if( sa->ball_owner == i )
      move_delay = MAX( move_delay, 
                        dist/soccer_env()->red_dribble_speed );
   else
      move_delay = MAX( move_delay,
                        dist/soccer_env()->red_speed );
 }
 if( sa->has_passed )
   pass_delay = v2_norm( v2_sub(sa->passer_pos, sa->kick_point))/
                        soccer_env()->red_pass_speed;

 return MAX( pass_delay, move_delay );
}


float saction_blue_elapsed_time( SoccerAction *sa )
{
 int i;
 float dist, move_delay = 0;
 float pass_delay = 0;

 for( i = 0; i < NPLAYERS; i++ ){
   dist = v2_norm(v2_sub( sa->pos[i], sa->move[i] ));
   if( sa->ball_owner == i )
      move_delay = MAX( move_delay, 
                        dist/soccer_env()->blue_dribble_speed );
   else
      move_delay = MAX( move_delay,
                        dist/soccer_env()->blue_speed );
 }
 if( sa->has_passed )
   pass_delay = v2_norm(v2_sub(sa->passer_pos, sa->kick_point))/
                        soccer_env()->blue_pass_speed;

 return MAX( pass_delay, move_delay );
}


void saction_red_act( SoccerState *s, SoccerAction *sa )
{
 int i, owner;

 if( sa->has_passed ){
   printf( "red passed %i \n", sa->ball_owner  );
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
 /*printf( "Red duration:%f  Blue duration:%f\n", saction_red_elapsed_time( red_act ),
                                                saction_blue_elapsed_time( blue_act ) );*/

 saction_simulate_red(s,red_act,dt);
 saction_simulate_blue(s,blue_act,dt);

 if( !sstate_is_inside_field( s, s->ball ) ){
   printf( "Out !!\n" );
   sstate_restart_game_pos(s);
 }
 
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

 if( (s->red_ball_owner < 0 ) && (s->blue_ball_owner < 0 ) &&
     (sstate_min_red_dist(s,s->ball) < sstate_min_blue_dist(s,s->ball)) &&
     (sstate_min_red_dist(s,s->ball) < .1 ) &&
     !red_act->has_passed ){
         s->ball_vel = v2_make(0,0);
         s->ball = s->red[ sstate_closest_red(s, s->ball) ];
         s->red_ball_owner = sstate_closest_red(s, s->ball);
 }

 if( red_act->has_kicked ){
     printf( "Red Goal Scored!! goal covering = %f \n", 
              red_act->enemy_goal_covering );
     if( red_act->enemy_goal_covering < .3 )
       sstate_restart_game_pos(s);
     return;
 }

 if( red_act->has_passed ){
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
 
     if( v2_norm( v2_sub( red_act->move[i], s->red[i] )) > .02 ) /* controller tol */
       s->red[i] = v2_add( s->red[i], v2_scale( dt*robot_speed,
                   v2_unit( v2_sub( red_act->move[i], s->red[i] ))) );

   } 
 }
}



void saction_simulate_blue( SoccerState *s, SoccerAction *blue_act, 
                            float dt )
{
 int i;

 if( (s->red_ball_owner < 0 ) && (s->blue_ball_owner < 0 ) &&
     (sstate_min_blue_dist(s,s->ball) < sstate_min_red_dist(s,s->ball)) &&
     (sstate_min_blue_dist(s,s->ball) < .15 ) ){
         s->ball_vel = v2_make(0,0);
         s->ball = s->blue[ sstate_closest_blue(s, s->ball) ];
         s->blue_ball_owner = sstate_closest_blue(s, s->ball);
 }

 if( blue_act->has_kicked  ){
   printf( "Blue Goal Scored!! goal covering = %f \n", 
            blue_act->enemy_goal_covering );
   if( blue_act->enemy_goal_covering < .3 )
     sstate_restart_game_pos(s);
   return; 
 }

 if( blue_act->has_passed ){
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


