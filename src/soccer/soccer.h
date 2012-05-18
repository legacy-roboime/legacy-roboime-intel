#ifndef SOCCER_H
#define SOCCER_H

#ifdef __cplusplus
extern "C" {
#endif 

#define NPLAYERS 5

#include "vector.h"
#include "soccer_env.h" 
#include <time.h>

#define DRAND()   ((float)rand()/(float)RAND_MAX)

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



typedef struct SoccerAction{
 Boolean prune;
 Vector2 kick_point;
 Vector2 move[NPLAYERS];  
 Vector2 pos[NPLAYERS];
 Vector2 passer_pos;
 Boolean has_kicked;
 Boolean has_passed;
 int ball_owner;
 int passer;
 float enemy_goal_covering;
} SoccerAction;


typedef struct SoccerState{

  float red_time_stamp;
  float blue_time_stamp;

  Vector2 ball;
  Vector2 ball_vel;
  Vector2 red[NPLAYERS];
  Vector2 blue[NPLAYERS];

  Boolean goal_scored;
  Boolean goal_received;

  float red_goal_covering;
  float blue_goal_covering;

  int red_ball_owner;
  int blue_ball_owner;

  int red_passer;
  int blue_passer;

} SoccerState;


/* Move table functions */

Vector2 get_red_move_table( int i );
Vector2 get_blue_move_table( int i );
void set_red_move_table( int i, Vector2 newpos );
void set_blue_move_table( int i, Vector2 newpos );

/* SoccerAction functions */

SoccerAction saction_red_make( SoccerState *s );
SoccerAction saction_blue_make( SoccerState *s );

float saction_red_elapsed_time( SoccerAction *sa );
float saction_blue_elapsed_time( SoccerAction *sa );

void saction_red_act( SoccerState *s , SoccerAction *sa );
void saction_blue_act( SoccerState *s , SoccerAction *sa );

void saction_simulate(  SoccerState *s, SoccerAction *red_act,
                         SoccerAction *blue_act, float dt );
void saction_simulate_red(  SoccerState *s, SoccerAction *red_act, 
                            float dt );
void saction_simulate_blue(  SoccerState *s, SoccerAction *blue_act, 
                             float dt );


/* Main SoccerState functions */

SoccerState* sstate_alloc( void );
float sstate_evaluate( SoccerState *s );
void sstate_restart_game_pos( SoccerState *s );

SoccerAction sstate_red_get_ball( SoccerState *s );
SoccerAction sstate_blue_get_ball( SoccerState *s );

SoccerAction sstate_red_receive_ball( SoccerState *s, int recv );
SoccerAction sstate_blue_receive_ball( SoccerState *s, int recv );

SoccerAction sstate_red_kick_to_goal( SoccerState *s );
SoccerAction sstate_blue_kick_to_goal( SoccerState *s );

SoccerAction sstate_red_pass( SoccerState *s, int recv, float recv_radius );
SoccerAction sstate_blue_pass( SoccerState *s, int recv, float recv_radius );

SoccerAction sstate_red_move( SoccerState *s, int robot, float radius );
SoccerAction sstate_blue_move( SoccerState *s, int robot, float radius );

Boolean sstate_is_valid_red_pos( SoccerState *s, int robot, Vector2 p );
Boolean sstate_is_valid_blue_pos( SoccerState *s, int robot, Vector2 p );
Boolean sstate_is_inside_field( SoccerState *s, Vector2 p );


/* Auxiliar SoccerState functions */

float time_to_intersect( Vector2 friend_pos, Vector2 friend_direction,
                         Vector2 enemy_pos, float friend_speed, float enemy_speed );

Boolean sstate_is_inside_field( SoccerState *s, Vector2 p );

float sstate_possible_red_pass( SoccerState *s, int recv, Vector2 p );
float sstate_possible_blue_pass( SoccerState *s, int recv, Vector2 p );

float sstate_min_red_dist( SoccerState *s, Vector2 p );
float sstate_min_blue_dist( SoccerState *s, Vector2 p );

int sstate_closest_red( SoccerState *s, Vector2 p );
int sstate_closest_blue( SoccerState *s, Vector2 p );

float sstate_time_to_red_get_ball( SoccerState *s, int *closest );
float sstate_time_to_blue_get_ball( SoccerState *s, int *closest );


#ifdef __cplusplus
}
#endif    

#endif
