#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <cmath>
#include <cfloat>

using namespace std;

#define RATE	6.2//2.50

#ifndef M_2PI
#define M_2PI	6.2831853071795865
#endif

#ifndef M_PI
#define M_PI	3.1415926535897932
#endif

#define M_INF   9E99

#ifndef MIN
#define MIN(x,y) (x < y ? x : y)
#endif

#ifndef MAX
#define MAX(x,y) (x > y ? x : y)
#endif

//Garante que x fique entre -MPI e MPI
#define __q(x)	((x) > M_PI ? (x) - M_2PI : -(x) > M_PI ? (x) + M_2PI : (x))
//qreal __n(qreal ang) {return ang > M_PI ? __n(ang - M_2PI) : -ang > M_PI ? __n(ang + M_2PI) : ang;}
#define SQ(x) ((x)*(x))

#define DEGTORAD(x) (M_PI * (x) / 180)
#define RADTODEG(x) (180 * (x) / M_PI)

#endif
