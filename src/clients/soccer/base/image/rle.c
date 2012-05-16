#include "rasterfile.h"
#include "image.h"


typedef struct RLEdatum {
  unsigned char cnt;
  Byte pix;
} RLEdatum;


void rle_read(int fd, Image *i)
{
  RLEdatum r;
  int u = 0, v = 0;

  while (eread(fd, &r, sizeof(RLEdatum))) {
    do {
      img_puti(i,u,v,r.pix);
      if (++u == i->w) {
	u = 0; v++;
      }
    } while (r.cnt--);
  }
}

void rle_write(int fd, Image *i)
{
  int u, v, c;
  RLEdatum r;

  for (v = 0; v < i->h; v++) {
    r.pix = img_geti(i, 0, v); r.cnt = 0;
    for (u = 1; u < i->w; u++) {
      if (r.cnt < 255 && (c = img_geti(i, u, v)) == r.pix) {
	r.cnt++;
      } else {
	write(fd, &r, sizeof(RLEdatum));
	r.pix = c; r.cnt = 0;
      }
    }
    ewrite(fd, &r, sizeof(RLEdatum));
  }
}

