#include<dos.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

// 19-02-2024

// screen width & height
#define SW 320
#define SH 200

typedef unsigned char byte;
typedef unsigned int uint;

typedef struct {
  byte** data;
  int w;
  int h;
} Bitmap;


byte far *VGA = (byte far*)0xa0000000L;
byte buffer[64000];

union REGS regs;
byte activepage;
byte columns = 79;
byte screenmode = 2;

void SCREEN(byte mode) {
  regs.h.ah = 0;
  regs.h.al = mode;

  int86(0x10, &regs, &regs);
}

void CLS() {
  memset(buffer, 0, (long) SW * SH);
}

void PIX(int x, int y, byte colour) {
  if (x < 0 || x >= 320 || y < 0 || y >= 320) return;
  if (colour == 13) return;

  buffer[(long)y * SW + x] = colour;
}

void flush_buf() {
  long size = (long)SW * SH;
  memcpy(VGA, buffer, size);
}

void getmode() {
  regs.h.ah = 15;
  int86(0x10, &regs, &regs);

  activepage = regs.h.bh;
  screenmode = regs.h.al;
  columns = regs.h.ah;
}

void gotoxy(uint x, uint y) {
  getmode();

  regs.h.ah = 2;
  regs.h.dl = x;
  regs.h.dh = y;
  regs.h.bh = activepage;

  int86(0x10, &regs, &regs);
}


void rect(int x, int y, int w, int h, byte colour) {
  int a, b;

  for (b = y; b <= y + h; b++)
  for (a = x; a <= x + w; a++)
    PIX(a, b, colour);
}


void LoadBMP(Bitmap *bitmap, char* filename) {
  byte buf[2];
  int w = 1, h = 1;
  int pad = 0, palsize = 0;
  int start;
  int byteidx;
  int a, b;
  int pair;

  FILE *fh;

  // free(bitmap->data);

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


void DebugBMP(Bitmap *bitmap) {
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

void DrawBMP(Bitmap *bitmap, int x, int y) {
  int a, b;

  for (b = 0; b < bitmap->h; b++)
  for (a = 0; a < bitmap->w; a++)
    PIX(x + a, y + b, bitmap->data[b][a]);
}

void DrawRegion(Bitmap *bitmap, int srcx, int srcy, int w, int h, int dx, int dy) {
  int a, b;

  for (b = 0; b < h; b++)
  for (a = 0; a < w; a++) {
    if (srcy + b >= bitmap->h ||
	srcx + a >= bitmap->w)
	continue;

    PIX(dx + a, dy + b, bitmap->data[srcy + b][srcx + a]);
  }
}

void sleep(double s) {
  clock_t start_t = clock();
  while ((double)(clock() - start_t) / CLOCKS_PER_SEC < s);
}



int file_exists(char* filename) {
  FILE *file;

  if ( (file = fopen(filename, "r")) != 0) {
    fclose(file);
    return 1;
  } else return 0;
}

int get_digit_count(int num) {
  return num == 0 ? 1 : log10(abs(num)) + 1;
}

byte* digits(int num) {
  int a, size;
  byte* ary;

  num = abs(num);
  size = get_digit_count(num);
  ary = (byte*) malloc(size);

  for (a = 0; a < size; a++) {
    ary[a] = num % 10;
    num /= 10;
  }

  return ary;
}


void draw_score(Bitmap* sprite, int score) {
  int a;
  int size = get_digit_count(score);
  byte* d = digits(score);

  for (a = 0; a < size; a++)
    DrawRegion(sprite,
      d[size - a - 1] * 6, 0,
      6, sprite->h,
      10 + (a * 6), 10);

  free(d);
}


int main() {
  int a, i;
  Bitmap score_spr, bird_spr;

  if (!file_exists("SCORESM2.BMP")) {
    printf("SCORESM2.BMP doesn't exist!\n");
    return 0;
  }

  //if (!file_exists("BIRD.BMP")) {
  //  printf("BIRD.BMP doesn't exist!\n");
  //  return 0;
  //}

  // Be sure to change the DIR first before loading the image
  LoadBMP(&score_spr, "SCORESM2.BMP");
  // LoadBMP(&bird_spr, "BIRD.BMP");
  // DebugBMP(&score_spr);

  // start mode 13h just like in QBASIC / VBDOS
  SCREEN(0x13);


  // DrawBMP(&score_spr, 10, 10 * a);
  for (i = 0; i <= 100; i++) {
    draw_score(&score_spr, i);

    flush_buf();

    sleep(0.01);
  }

  flush_buf();

  // gotoxy(0, 6);
  // printf("%.f ms\n", time_spent * 1000);

  getch();

  // back to text mode
  SCREEN(0x10);

  return 0;
}
