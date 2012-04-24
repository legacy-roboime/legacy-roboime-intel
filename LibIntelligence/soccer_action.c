#include "soccer.h"

SoccerAction saction_make( int ball_owner )
{
 int i;
 SoccerAction action;

 for( i = 0; i < NPLAYERS; i++ ) 
    action.move[i] = v2_make(0,0);
 action.has_kicked = FALSE; 
 action.has_passed = FALSE;
 action.ball_owner = ball_owner;
 return action;
}


void saction_act( SoccerState *s, SoccerAction *sa )
{
 int i, owner;

 if( sa->has_kicked ){
   printf( "# Goal Scored!!\n" );
   sstate_restart_game_pos(s);
   return; 
 }

 for( i = 0; i < NPLAYERS; i++ ) {
   s->red[i] = v2_add( s->red[i], sa->move[i] );
 //  if( v2_sqrnorm( sa->move[i] ) > EPS )
 //    printf( "# Robot %i moved\n", i );
 } 

 if( sa->has_passed )
    printf("# Pass %i -> %i\n", s->red_ball_owner,
           sa->ball_owner );

 if( (owner = sa->ball_owner) >= 0 ){
     if(s->red_ball_owner == -1) 
        printf( "# red %i get ball\n", sa->ball_owner );
     s->ball = s->red[owner]; 
     s->red_ball_owner = owner;
 } 


}

/*
float saction_simulate(  SoccerState *s, SoccerAction *red_act,
                         SoccerAction *blue_act, float dt )
{
 Vector2 ball_owner;
 int i, owner, elapsed_time = dt;

 if( red_act->has_kicked ){
   printf( "@ Red Goal Scored!!\n" );
   sstate_restart_game_pos(s);
   return elapsed_time; 
 }

 if( blue_act->has_kicked ){
   printf( "@ Blue Goal Scored!!\n" );
   sstate_restart_game_pos(s);
   return elapsed_time; 
 }

 if( red_act->has_passed ){
   ball_owner = s->red[red_act->ball_owner];
   elapsed_time = MAX( dt, v2_norm(v2_sub(ball_owner, s->ball))/s->red_pass_speed );
   s->ball = ball_owner;  
   s->red_ball_owner = red_act->ball_owner;
 }

 if( blue_act->has_passed ){
   ball_owner = s->blue[blue_act->ball_owner];
   elapsed_time = MAX( dt, v2_norm(v2_sub(ball_owner, s->ball))/s->blue_pass_speed );
   s->ball = ball_owner;  
   s->blue_ball_owner = blue_act->ball_owner;
 }

 for( i = 0; i < NPLAYERS; i++ ) {
   if( v2_norm(red_act->move[i]) <= elapsed_time*s->red_speed )
      s->red[i] = v2_add( s->red[i], v2_scale( elapsed_time*s->red_speed,
                          v2_unit(red_act->move[i])) );
   else 
      s->red[i] = v2_add( s->red[i], red_act->move[i] ); 

   if( v2_norm(blue_act->move[i]) <= elapsed_time*s->blue_speed )
      s->blue[i] = v2_add( s->blue[i], v2_scale( elapsed_time*s->blue_speed,
                          v2_unit(blue_act->move[i])) );
   else 
      s->blue[i] = v2_add( s->blue[i], blue_act->move[i] ); 
 } 

 return elapsed_time 
} */



