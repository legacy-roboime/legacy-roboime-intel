#include        "image.h"


#define QUANT_N(V,N) ((V/(256/N))*(255/(N-1)))

void img_dither_ds(Image *i, int n)
{
  int u, v, c, q, err = 0, w = i->w-1;

  if (img_type(i) != IMG_MONOCOL) 
    { warning("dither_ds implemented only for greyscale images"); return;}

  for (v = 0; v < i->h; v++) {
    for (u = EVEN(v)? 0 : w ; u >= 0 && u <= w; u = EVEN(v)? u+1 : u-1) {
      c = img_geti(i,u,v);
      q = c + err;
      q = QUANT_N(q,n);
      q = CLAMP(q,0,255);
      img_puti(i,u,v,q);
      err += c - q;
    }
  }
}
