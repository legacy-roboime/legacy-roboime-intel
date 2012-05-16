/*	image.h - image definitions	*/

#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"

#define IMG_MONOCOL 0
#define IMG_MAPPCOL 1
#define IMG_TRUECOL 2

typedef unsigned char Byte;

typedef struct Bcolor {
  Byte z, y, x;
} Bcolor;

typedef struct CMap {
  Byte r[256];
  Byte g[256];
  Byte b[256];
} CMap;

typedef struct Image {
  int type;
  int w, h;
  CMap  *m;
  union {
    Byte   *b;
    Bcolor *c;
  } u;
} Image;

#define img_type(_) (_)->type
#define img_cmap(_) (_)->m

Image *img_init(int type, int w, int h);
void  img_clear(Image *i, Color c);
Image *img_read(char *fname);
void  img_write(Image *i, char *fname, int cflag);

void  img_puti(Image *i, int u, int v, int c);
int   img_geti(Image *i, int u, int v);

void  img_putc(Image *i, int u, int v, Color c);
Color img_getc(Image *i, int u, int v);

void img_free(Image *i);

void  cmap_read(CMap *m, int maplength, int fd);
void  cmap_rbg(CMap *m);
void  cmap_gamma(CMap *m, Real gamval);

void  img_histogram(Image *i, int *hist, int nb);
Image *img_quant_popul(Image *i, int n, int nb);

void  img_dither_eds(Image *i, int n);


#endif




