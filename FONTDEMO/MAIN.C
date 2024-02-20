#include<dos.h>
#include<conio.h>  // getch
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#include "BITMAP.H"

// 20-02-2024
// Based on GBCLIKE but with the normal VGA resolution.

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

  delete(d);
}


int main() {
  int a, i;
  Bitmap ti84;

  if (!file_exists("TI84.BMP")) {
    printf("TI84.BMP doesn't exist!\n");
    return 0;
  }

  load_bmp(&ti84, "TI84.BMP");
  // debug_bmp(&ti84);

  // start mode 13h just like in QBASIC / VBDOS
  SCREEN(0x13);

  for (i = 0; i <= 20; i++) {
    fill_buf(1);
    draw_str(&ti84, itoa(i), 10, 10);

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
