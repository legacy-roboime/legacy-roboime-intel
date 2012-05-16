#include "image.h"


void cmap_read(CMap *m, int maplength, int fd)
{
  read(fd, m->r, maplength/3);
  read(fd, m->g, maplength/3);
  read(fd, m->b, maplength/3);
}

void cmap_rgb(CMap *m)
{
  int k;
  for (k = 0; k < 256; k++) {
    Color c = index_to_rgb(k, 3, 3, 2);
    m->r[k] = RED(c);
    m->g[k] = GRN(c);
    m->b[k] = BLU(c);
  }
}

void cmap_gamma(CMap *m, Real gamval)
{
  int k;
  for (k = 0; k < 256; k++) {
    m->r[k] = 255 * pow(m->r[k]/255., 1./gamval);
    m->g[k] = 255 * pow(m->g[k]/255., 1./gamval);
    m->b[k] = 255 * pow(m->b[k]/255., 1./gamval);
  }
}


#define SQR_NORM(R,G,B) (SQR(R) + SQR(G) + SQR(B))

int cm_closest(CMap *m, Color c)
{
  int d, dmin, k, kmin, r = RED(c), g = GRN(c), b = BLU(c);
  
  for (k = 0; k < 256; k++) {
    int dr = m->r[k] - r, dg = m->g[k] - g, db = m->b[k] - b;
    d = SQR_NORM(dr, dg, db);
    if (d < dmin || k == 0) {
      dmin = d;
      kmin = k;
    }
  }
  return kmin;
}

