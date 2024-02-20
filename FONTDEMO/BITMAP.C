#include<stdio.h>
#include<stdlib.h>  // malloc

#include "BITMAP.H"

void load_bmp(Bitmap *bitmap, char* filename) {
  byte buf[2];
  int w = 1, h = 1;
  int pad = 0, palsize = 0;
  int start;
  int byteidx;
  int a, b;
  int pair;

  FILE *fh;

  fh = fopen(filename, "rb");

  fseek(fh, 18, SEEK_SET);
  fread(buf, 1, 1, fh);
  w = buf[0];
  bitmap->w = w;

  fseek(fh, 22, SEEK_SET);
  fread(buf, 1, 1, fh);
  h = buf[0];
  bitmap->h = h;

  // initialise bitmap data
  bitmap->data = (byte**) malloc(h * sizeof(byte *));
  for (a = 0; a < h; a++)
    bitmap->data[a] = (byte*) malloc(w * sizeof(byte));

  fseek(fh, 50, SEEK_SET);
  fread(buf, 1, 2, fh);
  palsize = buf[1] * 256 + buf[0];
  pad = (4 - w % 4) % 4;

  start = 53 + palsize * 4;

  for (b = 0; b < h; b++)
  for (a = 0; a < w; a++) {
    byteidx = start + (b * (w + pad)) + a;

    fseek(fh, byteidx, SEEK_SET);
    fread(buf, 1, 1, fh);
    bitmap->data[h - b - 1][a] = buf[0];
  }

  fclose(fh);
}


void debug_bmp(Bitmap *bitmap) {
  int a, b;

  printf("w: %d, h: %d\n", bitmap->w, bitmap->h);

  // printf("palsize: %d\n", palsize);

  for (b = 0; b < bitmap->h; b++) {
  for (a = 0; a < bitmap->w; a++)
    printf("%d ", bitmap->data[b][a]);
    printf("\n");
  }

  getch();
}

void draw_bmp(Bitmap *bitmap, int x, int y) {
  int a, b;

  for (b = 0; b < bitmap->h; b++)
  for (a = 0; a < bitmap->w; a++)
    PSET(x + a, y + b, bitmap->data[b][a]);
}

void draw_region(Bitmap *bitmap, int srcx, int srcy, int w, int h, int dx, int dy) {
  int a, b;

  for (b = 0; b < h; b++)
  for (a = 0; a < w; a++) {
    if (srcy + b >= bitmap->h ||
	srcx + a >= bitmap->w)
	continue;

    PSET(dx + a, dy + b, bitmap->data[srcy + b][srcx + a]);
  }
}

// Draw string using TI84.BMP
void draw_str(Bitmap *font, char* text, int x, int y) {
  int a, charcode;
  int col, row;
  int size = strlen(text);

  for (a = 0; a < size; a++) {
    charcode = (int) text[a];
    col = charcode % 16;
    row = charcode / 16;

    draw_region(
      font,
      col * 6, row * 8,
      6, 8,
      x + a * 6, y);
  }
}