#include  <math.h>
#include  "image.h"

#define HIST_SIZE(n) (int)(pow(2.0, 3.0*(n)))

void img_histogram(Image *i, int *hist, int nb)
{
  int u, v, k, b, hsize = HIST_SIZE(nb);

  for (k = 0; k < hsize; k++)
    hist[k] = 0;
  for (v = 0; v < i->h; v++) {
    for (u = 0 ; u < i->w; u++) {
      Color c = img_getc(i,u,v);
      hist[rgb_to_index(c, nb, nb, nb)] += 1;
    }
  }
}


static void cmap_select(int n, int *hist, int nb, CMap *s)
{
  Color c;
  int k, l, cmax, imax, hsize = HIST_SIZE(nb);
  
  for (k = 0; k < n; k++) {
    cmax = 0;
    for (l = 0; l < hsize; l++) {
      if (hist[l] > cmax) {
	cmax = hist[l];
	imax = l;
      }
    }
    c = index_to_rgb(imax, nb, nb, nb);
    s->r[k] = RED(c);
    s->g[k] = GRN(c);
    s->b[k] = BLU(c);
    hist[imax] = -hist[imax];
  }
  for (k = n; k < 256; k++) {
    s->r[k] = s->g[k] = s->b[k] = 0;
  }
}

#if 0
Image *img_quant_popul(Image *i, int n, int nb)
{
  int u, v, k, *hist = NEWTARRAY(HIST_SIZE(nb), int);
  Image *q = img_init(IMG_MAPPCOL, i->w, i->h);

  img_histogram(i, hist, nb);
  cmap_select(n, hist, nb, img_cmap(q));

  for (v = 0; v < i->h; v++) {
    for (u = 0 ; u < i->w; u++) {
      k = cm_closest(img_cmap(q), img_getc(i, u, v));
      img_puti(q, u, v, k);
    }
  }
  efree(hist);
  return q;
}
#else
Image *img_quant_popul(Image *i, int n, int nb)
{
  int u, v, k, *hist = NEWTARRAY(HIST_SIZE(nb), int);
  Image *q = img_init(IMG_MAPPCOL, i->w, i->h);

  img_histogram(i, hist, nb);
  cmap_select(n, hist, nb, img_cmap(q));

  for (k = 0; k < HIST_SIZE(nb); k++)
    if (hist[k] != 0)
      hist[k] = cm_closest(img_cmap(q), index_to_rgb(k, nb, nb, nb));
  for (v = 0; v < i->h; v++) {
    for (u = 0 ; u < i->w; u++) {
      k = hist[rgb_to_index(img_getc(i, u, v), nb, nb, nb)];
      img_puti(q, u, v, k);
    }
  }
  efree(hist);
  return q;
}
#endif
