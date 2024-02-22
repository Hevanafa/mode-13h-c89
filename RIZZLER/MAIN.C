#include<dos.h>     // int86
#include<conio.h>   // getch
#include<stdio.h>   // printf
#include<stdlib.h>  // malloc, free, itoa
#include<time.h>    // clock
#include<math.h>    // log10
// #include<limits.h>  // CHAR_MIN

#include "BITMAP.H"

// 20-02-2024
// based on SCORESPR with a smaller buffer size

Bitmap bg, font;

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
  return num == 0 ? 1 : (int)log10(abs(num)) + 1;
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
    draw_region(sprite,
      d[size - a - 1] * 6, 0,
      6, sprite->h,
      10 + (a * 6), 10);

  delete(d);
}


int main() {
  int i;
  char* s;

  if (!file_exists("TI84SM.BMP")) {
    printf("TI84SM.BMP doesn't exist!\n");
    return 0;
  }

  if (!file_exists("THUMB.BMP")) {
    printf("THUMB.BMP doesn't exist!\n");
    return 0;
  }

  // Fixed: This caused serious disk error ??
  // Solution: Don't forget to include the header files
  // Be sure to change the DIR first before loading the image
  load_bmp(&font, "TI84SM.BMP");
  load_bmp(&bg, "THUMB.BMP");
  // DebugBMP(&score_spr);

  // start mode 13h just like in QBASIC / VBDOS
  SCREEN(0x13);

  // DrawBMP(&score_spr, 10, 10 * a);
  fill_buf(1);

  // draw_bmp(&font, 0, 0);

  if (!font.is_set) {
    printf("font is NULL!");
    return 0;
  }

  draw_bmp(&bg, 0, 0);

  draw_str(&font, "Sticking out your", 10, 18);
  draw_str(&font, "gyatt for the", 10, 26);
  draw_str(&font, "rizzler", 10, 34);
  draw_str(&font, "You're so Skibidi", 10, 42);
  draw_str(&font, "You're so Fanum Tax", 10, 50);
  draw_str(&font, "I just wanna be", 10, 82);
  draw_str(&font, "your Sigma", 10, 90);
  draw_str(&font, "Freaking come here", 10, 98);
  draw_str(&font, "Give me your Ohio", 10, 106);

  flush_buf();
  // sleep(0.01);

  // gotoxy(0, 6);
  // printf("%.f ms\n", time_spent * 1000);

  getch();

  // back to text mode
  SCREEN(0x10);

  return 0;
}
