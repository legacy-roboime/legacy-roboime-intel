#ifndef SOCCER_ENV_H
#define SOCCER_ENV_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "defs.h"

typedef struct SoccerEnvironment{
  float red_speed;
  float blue_speed;
  float red_dribble_speed;
  float blue_dribble_speed;
  float red_pass_speed;
  float blue_pass_speed;
  float robot_radius;
  float goal_size;
  float field_w, field_h;
  Boolean left_red_side;
} SoccerEnvironment;

void soccer_env_init( void );
SoccerEnvironment* soccer_env(void);

#ifdef __cplusplus
}
#endif    

#endif
