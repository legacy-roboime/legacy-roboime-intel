#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <cmath>

#define RATE	6.2//2.50
#define M_2PI	6.2831853071795865
#define M_PI	3.1415926535897932
//Garante que x fique entre -MPI e MPI
#define __q(x)	((x) > M_PI ? (x) - M_2PI : -(x) > M_PI ? (x) + M_2PI : (x))
//qreal __n(qreal ang) {return ang > M_PI ? __n(ang - M_2PI) : -ang > M_PI ? __n(ang + M_2PI) : ang;}

#endif