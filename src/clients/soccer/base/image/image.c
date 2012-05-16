
#include "image.h"
#include "rasterfile.h"


#define IMG_RASTYPE(h) \
  ((h.ras_depth == 24)? IMG_TRUECOL : \
   (h.ras_depth == 8 && h.ras_maptype == RMT_EQUAL_RGB)? IMG_MAPPCOL : \
   IMG_MONOCOL)

static void ras_header(struct rasterfile *h, Image *i, int cflag);
static void read_sunheader(int fd, struct rasterfile *h);
static void write_sunheader(int fd, struct rasterfile *h);
static void read_sun_long(int *l, int fd);
static void write_sun_long(unsigned int l, int fd);

static void sunras_read(int fd, int depth, int w, int h, void *b);
static void sunras_write(int fd, int depth, int w, int h, void *b);
extern void rle_read(int fd, Image *i);
extern void rle_write(int fd, Image *i);


#define PIX_MIN 0
#define PIX_MAX 255

#define PIXVAL(I,U,V) I->u.b[U + (((I->h - 1) - (V)) * I->w)]

#define PIXRED(I,U,V) I->u.c[U + (((I->h - 1) - (V)) * I->w)].x
#define PIXGRN(I,U,V) I->u.c[U + (((I->h - 1) - (V)) * I->w)].y
#define PIXBLU(I,U,V) I->u.c[U + (((I->h - 1) - (V)) * I->w)].z


Image *img_init(int type, int w, int h)
{
  Image *i = NEWSTRUCT(Image);
  
  i->w = w; i->h = h; i->m = NULL;
  switch (i->type = type) {
  case IMG_TRUECOL:
    i->u.c = NEWTARRAY(w*h, Bcolor);
    break;
  case IMG_MAPPCOL:
    cmap_rgb(i->m = NEWSTRUCT(CMap));
  case IMG_MONOCOL:
    i->u.b = NEWTARRAY(w*h, Byte);
    break;
  default:
    error("(img init) wrong type");
  }
  img_clear(i, C_BLACK);
  return i;
}


void img_clear(Image *i, Color c)
{
  int u, v, k;

  switch (img_type(i)) {
  case IMG_MONOCOL: k = rgb_to_y(c); break;
  case IMG_MAPPCOL: k = cm_closest(i->m,c); break;
  case IMG_TRUECOL: default:;
  }
  for (v = 0; v < i->h; v++)
    for (u = 0; u < i->w; u++)
      if (img_type(i) == IMG_TRUECOL)
	img_putc(i,u,v,c);
      else
	img_puti(i,u,v,k);
}


Image *img_read(char *fname)
{
  struct rasterfile h;
  int fd;
  Image *i;
  
  fd =  (strcmp("stdin", fname) == 0)? 0 : eopen(fname, 0);
  read_sunheader(fd, &h);
  if (h.ras_magic != RAS_MAGIC)
    error("(img_read) wrong format");
  i = img_init(IMG_RASTYPE(h), h.ras_width, h.ras_height);

  if (h.ras_maptype == RMT_EQUAL_RGB)
    cmap_read(i->m, h.ras_maplength, fd);

  switch (h.ras_type) {
  case RT_FORMAT_RGB:
    sunras_read(fd, h.ras_depth, i->w, i->h, i->u.c); break;
  case RT_STANDARD: 
    sunras_read(fd, h.ras_depth, i->w, i->h, i->u.b); break;
  case RT_SIMPLE_RLE:
    rle_read(fd, i); break;
  default: 
    error("(img_read) unknown format");
  }
  close(fd);
  return i;
}


void img_write(Image *i, char *fname, int cflag)
{
  struct rasterfile h;
  int fd;

  fd = (strcmp("stdout", fname) == 0)? 1 : ecreat(fname, 0666);

  ras_header(&h, i, cflag);
  write_sunheader(fd, &h);

  if (img_type(i) == IMG_MAPPCOL && i->m != NULL)
    write(fd, i->m, sizeof(CMap));

  switch (h.ras_type) {
  case RT_FORMAT_RGB:
    sunras_write(fd, h.ras_depth, i->w, i->h, i->u.c); break;
  case RT_STANDARD: 
    sunras_write(fd, h.ras_depth, i->w, i->h, i->u.b); break;
  case RT_SIMPLE_RLE: 
    rle_write(fd, i); break;
  }
  close(fd);
}


void img_puti(Image *i, int u, int v, int c)
{
  if (u >= 0 && u < i->w && v >= 0 && v < i->h)
    switch (img_type(i)) {
    case IMG_TRUECOL:
      PIXRED(i,u,v) = PIXGRN(i,u,v) = PIXBLU(i,u,v) = CLAMP(c,PIX_MIN,PIX_MAX);
      break;
    case IMG_MAPPCOL: case IMG_MONOCOL:
      PIXVAL(i,u,v) = CLAMP(c, PIX_MIN, PIX_MAX);
      break;
    }
}


int img_geti(Image *i, int u, int v)
{
  if (u >= 0 && u < i->w && v >= 0 && v < i->h) 
    switch (img_type(i)) {
    case IMG_TRUECOL:
      return (PIXRED(i,u,v) + PIXGRN(i,u,v) + PIXBLU(i,u,v)) / 3;
    case IMG_MAPPCOL: case IMG_MONOCOL:
      return PIXVAL(i,u,v);
    }
  else return PIX_MIN;
}


void img_putc(Image *i, int u, int v, Color c)
{
  switch (img_type(i)) {
  case IMG_TRUECOL:
    if (u >= 0 && u < i->w && v >= 0 && v < i->h) {
      PIXRED(i,u,v) = CLAMP(RED(c), PIX_MIN, PIX_MAX); 
      PIXGRN(i,u,v) = CLAMP(GRN(c), PIX_MIN, PIX_MAX); 
      PIXBLU(i,u,v) = CLAMP(BLU(c), PIX_MIN, PIX_MAX);
    }
    break;
  case IMG_MAPPCOL:
    img_puti(i, u, v, cm_closest(i->m,c)); break;
  case IMG_MONOCOL:
    img_puti(i, u, v, rgb_to_y(c)); break;
  }
}

Color img_getc(Image *i, int u, int v)
{
  int k;
  switch (img_type(i)) {
  case IMG_TRUECOL:
    if (u >= 0 && u < i->w && v >= 0 && v < i->h)
      return c_make(PIXRED(i,u,v),PIXGRN(i,u,v),PIXBLU(i,u,v));
    else
      return C_BLACK;
  case IMG_MAPPCOL:
    k = img_geti(i, u, v);
    return c_make(i->m->r[k], i->m->g[k], i->m->b[k]);
    break;
  case IMG_MONOCOL: 
    k = img_geti(i, u, v);
    return c_make(k, k, k);
  }
}

void img_free(Image *i)
{
  efree(i->u.c); efree(i->m); efree(i);
}

/* sunras */


static void ras_header(struct rasterfile *h, Image *i, int cflag)
{
  h->ras_magic = RAS_MAGIC;
  h->ras_width = i->w;
  h->ras_height = i->h;
  h->ras_depth = (img_type(i) == IMG_TRUECOL)? 24 : 8;
  h->ras_length = i->w * i->h;
  h->ras_type = (cflag)? RT_SIMPLE_RLE : RT_STANDARD;
  h->ras_maptype = (img_type(i) == IMG_MAPPCOL)? RMT_EQUAL_RGB : RMT_NONE;
  h->ras_maplength = (img_type(i) == IMG_MAPPCOL)? sizeof(CMap) : 0;
}

static void read_sunheader(int fd, struct rasterfile *h)
{
  read_sun_long(&(h->ras_magic), fd);
  read_sun_long(&(h->ras_width), fd);
  read_sun_long(&(h->ras_height), fd);
  read_sun_long(&(h->ras_depth), fd);
  read_sun_long(&(h->ras_length), fd);
  read_sun_long(&(h->ras_type), fd);
  read_sun_long(&(h->ras_maptype), fd);
  read_sun_long(&(h->ras_maplength), fd);
}

static void write_sunheader(int fd, struct rasterfile *h)
{
  write_sun_long(h->ras_magic, fd);
  write_sun_long(h->ras_width, fd);
  write_sun_long(h->ras_height, fd);
  write_sun_long(h->ras_depth, fd);
  write_sun_long(h->ras_length, fd);
  write_sun_long(h->ras_type, fd);
  write_sun_long(h->ras_maptype, fd);
  write_sun_long(h->ras_maplength, fd);
}

static void read_sun_long(int *l, int fd)
{
  unsigned char c0, c1, c2, c3;

  read(fd,&c0,1); read(fd,&c1,1); read(fd,&c2,1); read(fd,&c3,1);

  *l = (((unsigned int) c0 & 0xff) << 24) |
       (((unsigned int) c1 & 0xff) << 16) |
       (((unsigned int) c2 & 0xff) <<  8) |
       (((unsigned int) c3 & 0xff));
}

static void write_sun_long(unsigned int l, int fd)
{
    unsigned char c;
    c = ((l >> 24) & 0xff); write(fd, &c, 1);
    c = ((l >> 16) & 0xff); write(fd, &c, 1);
    c = ((l >> 8) & 0xff); write(fd, &c, 1);
    c = (l & 0xff); write(fd, &c, 1);
}


static void sunras_read(int fd, int d, int w, int h, void *buf)
{
  int i, lsize, padline;
  char pad[1], *bp;

  lsize = (w * d)/8;
  padline = ((w * d) % 16)? TRUE : FALSE;
  for (i = 0, bp = buf; i < h; i++, bp += lsize) {
    read(fd, bp, lsize);
    if (padline) read(fd, &pad, 1);
  }
}

static void sunras_write(int fd, int d, int w, int h, void *buf)
{
  int i, lsize, padline;
  char pad[1], *bp;

  lsize = (w * d)/8;
  padline = ((w * d) % 16)? TRUE : FALSE;
  for (i = 0, bp = buf; i < h; i++, bp += lsize) {
    write(fd, bp, lsize);
    if (padline) write(fd, &pad, 1);
  }
}

