#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include<time.h>

// This can be compiled with Microsoft C++ 7.0 (not Visual C++)
// 15-02-2024

#define SW 320
#define SH 200

typedef unsigned char byte;
typedef unsigned int uint;


// segment A000:0000
byte far *VGA = (byte far*)0xa0000000L;

union REGS regs;  // programming model 8088
byte activepage;  // current video screen
byte columns = 79;
byte screenmode = 2;

byte buffer[60000];  // should be 320x200, this is less

void SCREEN(byte mode) {
  regs.h.ah = 0;     // set the video mode
  regs.h.al = mode;

  int86(0x10, &regs, &regs);
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

void CLS() {
  // memset(VGA, 0, SW * SH);
  int a, b;

  for (b = 0; b < SH; b++)
  for (a = 0; a < SW; a++)
    // VGA[b * SW + a] = 0;
    buffer[b * SW + a] = 0;
}


void PSET(int x, int y, byte colour) {
  if (x < 0 || x >= 320 || y < 0 || y >= 100) return;

  // write directly to the video memory
  buffer[y * SW + x] = colour;
}


void rect(int x, int y, int w, int h, byte colour) {
  int a, b;

  for (b = y; b < y + h; b++)
  for (a = x; a < x + w; a++)
    PSET(a, b, colour);
}


void flush() {
  int a;
  for (a = 0; a < 30000; a++)
    VGA[a] = buffer[a];
}


int main() {
  int a, b, ch;
  float gravity = 0.02;
  float x = 50, y = 50;
  float vx = 0, vy = 0;
  int t;

  time_t now = time(0);
  struct tm *timeinfo = localtime(&now);
  long last_t = timeinfo->tm_sec;
  int last_fps = 0, fps = 0;

  int colour;

  SCREEN(0x13);

  while (1) {
    // update
    y += vy;
    vy += gravity;

    if (y >= 180) {
      vy = 0;
    }

    t = (t + 1) % 24;

    if (last_t != time(0) % 86400) {
      last_t = time(0) % 86400;
      last_fps = fps;
      fps = 1;
    } else {
      fps++;
    }

    // draw
    // CLS();
    // rect((int) x, (int) y, 8, 8, 15);

    for (a = 0; a < 32; a++) {
      colour = 32 + (t + a) % 24;
      rect(a * 10, 40, 10, 40, (byte) colour);
    }

    flush();



    // gotoxy doesn't exist in MS C 7.0
    // gotoxy(1, 1);
    // printf("x: %d, y: %d", x, y);

    //for (b = 0; b < 16; b++)
    //for (a = 0; a < 16; a++)
    // PSET(a, b + 30, (byte)(b * 16 + a));
    //	rect(a * 8, (b + 2) * 8, 8, 8, (byte)(b * 16 + a));


    // based on setcurs(col, row)
    gotoxy(0, 23);
    // printf("last_t: %d", last_t);
    printf("FPS: %d", last_fps);

    gotoxy(0, 24);
    printf("Esc - Exit");

    if (kbhit()) {
      ch = getch();

      if (ch == 0 || ch == 224) {
        ch = getch();

        switch (ch) {
          case 72: y -= 5; break;
          case 80: y += 5; break;

          case 75: x -= 5; break;
          case 77: x += 5; break;
        }
      }

      if (ch == 27) break;
      if (ch == 32) vy = -2;
    }

    // doesn't exist on MS C 7.0
    // delay(30);

  }

  // getch();

  // back to text mode
  SCREEN(0x10);

  return 0;
}
