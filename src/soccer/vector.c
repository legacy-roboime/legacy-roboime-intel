#include "vector.h"


Vector2 v2_make( Real x, Real y )
{
 Vector2 u;
 u.x = x;
 u.y = y;
 return u;
}


Vector2 v2_scale( Real scale, Vector2 v )
{
 Vector2 u;
 u.x = v.x * scale;
 u.y = v.y * scale;
 return u;
}


Vector2 v2_add( Vector2 v1, Vector2 v2 )
{
 Vector2 u;
 u.x = v1.x + v2.x;
 u.y = v1.y + v2.y;
 return u;
}


Vector2 v2_sub( Vector2 v1, Vector2 v2 )
{
 Vector2 u;
 u.x = v1.x - v2.x;
 u.y = v1.y - v2.y;
 return u;
}


Real v2_dot( Vector2 v1, Vector2 v2 )
{
 return v1.x*v2.x + v1.y*v2.y;
}


Real v2_sqrnorm( Vector2 v )
{
 return v.x*v.x + v.y*v.y;
}


Real v2_norm( Vector2 v )
{
 return sqrt( v2_sqrnorm( v ) );
}

//TODO: take a look at this:
const Vector2 v2_unit_default = {1.0, 0.0};

Vector2 v2_unit( Vector2 v )
{
  Real  length = v2_norm(v);
  if(fabs(length) < EPS) {
    printf("(v2_unit) zero norm\n");
    return v2_unit_default;
  } else
    return v2_scale(1.0/length, v);
}


Vector2 v2_ortho( Vector2 v )
{
 return v2_make( v.y, -v.x );
}


Vector2 v2_lerp(Real t, Vector2 a, Vector2 b)
{
  return v2_add(v2_scale((1-t), a), v2_scale((t), b));
}


Vector2 v2_bilerp(Real t, Real lt, Vector2 l0, Vector2 l1,
		          Real rt, Vector2 r0, Vector2 r1)
{
  return v2_lerp(t, v2_lerp(lt, l0, l1), v2_lerp(rt, r0, r1));
}


Box2d   b2_make( Vector2 ll, Vector2 ur )
{
 Box2d b;

 b.ll = ll;
 b.ur = ur;
 return b;
}

Boolean b2_inside( Box2d b, Vector2 p )
{
  return(    p.x > b.ll.x && p.x < b.ur.x
	  && p.y > b.ll.y && p.y < b.ur.y );
}


void b2_adjust( Box2d *b )
{
 Vector2 ll, ur;

 ll = v2_make( MIN( b->ll.x, b->ur.x),
               MIN( b->ll.y, b->ur.y));

 ur = v2_make( MAX( b->ll.x, b->ur.x),
               MAX( b->ll.y, b->ur.y));
 b->ll = ll;
 b->ur = ur;
}
