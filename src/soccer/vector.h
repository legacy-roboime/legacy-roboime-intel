#ifndef VECTOR_H
#define VECTOR_H

#include "defs.h"
#include <math.h>


typedef struct Vector2{
  Real x, y;
} Vector2;


typedef struct Box2d {
  Vector2 ll, ur;
} Box2d;


Vector2 v2_make( Real x, Real y );
Vector2 v2_scale( Real scale, Vector2 v );
Vector2 v2_add( Vector2 v1, Vector2 v2 );
Vector2 v2_sub( Vector2 v1, Vector2 v2 );
Real    v2_dot( Vector2 v1, Vector2 v2 );
Real    v2_sqrnorm( Vector2 v );
Real    v2_norm( Vector2 v );
Vector2 v2_unit( Vector2 v );
Vector2 v2_ortho( Vector2 v );

Vector2 v2_lerp( Real t, Vector2 a, Vector2 b );
Vector2 v2_bilerp(Real t, Real lt, Vector2 l0, Vector2 l1,
		          Real rt, Vector2 r0, Vector2 r1);

Box2d   b2_make( Vector2 ll, Vector2 ur );
Boolean b2_inside(Box2d b, Vector2 p);
void    b2_adjust(Box2d *b); 

#endif



  
    
