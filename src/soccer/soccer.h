#ifndef SOCCER_H
#define SOCCER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vector.h"
#include "soccer_env.h"
#include <time.h>

#define NPLAYERS 5

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
 Vector2 move[NPLAYERS];  
 Boolean has_kicked;
 Boolean has_passed;
 Vector2 kick_point;
 int ball_owner;
 Boolean get_ball;
} SoccerAction;


typedef struct SoccerState{

  Vector2 ball;
  Vector2 ball_vel;
  Vector2 red[NPLAYERS];
  Vector2 blue[NPLAYERS];

  Boolean goal_scored;
  Boolean goal_received;

  int red_ball_owner;
  int blue_ball_owner;

} SoccerState;


/* SoccerAction functions */

SoccerAction saction_make( int ball_owner );
void saction_act( SoccerState *s , SoccerAction *sa );
float saction_simulate(  SoccerState *s, SoccerAction *red_act,
                         SoccerAction *blue_act, float dt );

/* Auxiliar functions */
float time_to_intersect( Vector2 friend_, Vector2 friend_direction,
                         Vector2 enemy, float friend_speed, float enemy_speed );

/* SoccerState functions */

SoccerState* sstate_alloc( void );
float sstate_evaluate( SoccerState *s );
void sstate_restart_game_pos( SoccerState *s );

SoccerAction sstate_red_get_ball( SoccerState *s );
void sstate_blue_get_ball( SoccerState *s );

SoccerAction sstate_red_receive_ball( SoccerState *s, int recv ); 
void sstate_blue_receive_ball( SoccerState *s, int recv );

SoccerAction sstate_red_kick_to_goal( SoccerState *s );
void sstate_blue_kick_to_goal( SoccerState *s );

SoccerAction sstate_red_pass( SoccerState *s, int recv, float recv_radius );
void sstate_blue_pass( SoccerState *s, int recv, float recv_radius );

float sstate_possible_red_pass( SoccerState *s, int recv, Vector2 p );
float sstate_possible_blue_pass( SoccerState *s, int recv, Vector2 p );

SoccerAction sstate_red_move( SoccerState *s, float radius );
void sstate_blue_move( SoccerState *s, float radius );

Boolean sstate_is_inside_field( SoccerState *s, Vector2 p );

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
