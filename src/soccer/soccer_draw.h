#ifndef SOCCER_DRAW
#define SOCCER_DRAW

#ifdef __cplusplus
extern "C" {
#endif 

#ifdef HAVE_MACOSX
#include <GLUT/GLUT.h>
#else
#include <GL/glut.h>
#endif
#include "soccer.h"

void soccer_redraw( SoccerState *s );

#ifdef __cplusplus
}
#endif    

#endif

