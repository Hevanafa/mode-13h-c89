#include<stdio.h>
#include<stdlib.h>  // malloc
// #include<stdbool.h>

#include "BITMAP.H"

#define false 0
#define true !(false)
#define FONT_W 6
#define FONT_H 8

void load_bmp(Bitmap *bitmap, byte* filename) {
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

  bitmap->is_set = true;
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

void draw_char(Bitmap *font, byte c, int x, int y) {
  int col, row;

  // printf("%d %d, ", c % 16, c / 16 - 2);
  // return;

  // skip everything below 32 including space
  // skip extended ASCII
  if (c <= ' ' || c > 126) return;

  col = c % 16;
  row = c / 16 - 2;

  // printf("%d %d, ", col, row);
  // return;

  draw_region(
    font,
    col * FONT_W, row * FONT_H,
    FONT_W, FONT_H,
    x, y);
}

// Draw string using TI84.BMP
void draw_str(Bitmap *font, byte* text, int x, int y) {
  int a;
  int size = strlen(text);

  if (!font->is_set) {
    printf("font is NULL\n");
    return;
  }

  for (a = 0; a < size; a++)
    draw_char(font, text[a], x + a * FONT_W, y);
}
