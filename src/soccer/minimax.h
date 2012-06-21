#ifndef MINIMAX_H
#define MINIMAX_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "soccer.h"
#include "soccer_draw.h"

#define MAX_NPLAYS 50
#define MIN_NPLAYS 50

#define MINIMAX_MAX_LEVEL 2

void minimax_init( SoccerState *s );

void minimax_use_next_red_robot(void);
void minimax_use_next_blue_robot(void);

SoccerAction* minimax_get_best_red_action( void );
SoccerAction* minimax_get_best_blue_action( void );

void  minimax_play( SoccerState *s, int depth );
void  minimax_playMax( SoccerState *s, int depth );
void  minimax_playMin( SoccerState *s, int depth );

float minimax_getMaxValue( SoccerState s, int depth, 
                           float parent_duration, float alpha, float beta );
float minimax_getMinValue(SoccerState s, int depth, 
                           float parent_duration, float alpha, float beta );

float minimax_red_time_weight_func( SoccerState *s );
float minimax_blue_time_weight_func( SoccerState *s );

SoccerAction minimax_expandMax( SoccerState *s, int i, int depth );
SoccerAction minimax_expandMin( SoccerState *s, int i, int depth );
 
#ifdef __cplusplus
}
#endif    

#endif
