#include<stdio.h>  // printf
#include<dos.h>    // REGS
#include<conio.h>  // getch
#include<time.h>   // time

// By Hevanafa, 15-02-2024
// This can be compiled with Microsoft C++ 7.0 (not Visual C++).
// Requires Large/Huge memory model to run.

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

byte buffer[(long) SW * SH];	// 320x200

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
  memset(buffer, 0, 64000);
}


void PSET(int x, int y, byte colour) {
  if (x < 0 || x >= 320 || y < 0 || y >= 200) return;

  buffer[y * SW + x] = colour;
}


void rect(int x, int y, int w, int h, byte colour) {
  int a, b;

  for (b = y; b < y + h; b++)
  for (a = x; a < x + w; a++)
    PSET(a, b, colour);
}


void flush() {
  long size = (long)SW * SH;
  memcpy(VGA, buffer, size);
}


int main() {
  int a, b, ch;
  int y = 50;
  int t;

  // for FPS counter
  time_t now = time(0);
  struct tm *timeinfo = localtime(&now);
  long last_t = timeinfo->tm_sec;
  int last_fps = 0, fps = 0;

  int colour;

  SCREEN(0x13);

  while (1) {
    // update
    t = (t + 1) % 24;

    if (last_t != time(0) % 86400) {
      last_t = time(0) % 86400;
      last_fps = fps;
      fps = 1;
    } else {
      fps++;
    }


    // draw
    CLS();

    // draw the rainbow bar
    for (a = 0; a < 32; a++) {
      colour = 32 + (t + a) % 24;
      rect(a * 10, y, 10, 40, (byte) colour);
    }

    flush();

    // based on setcurs(col, row)
    gotoxy(0, 23);
    // printf("last_t: %d", last_t);
    printf("FPS: %d", last_fps);

    gotoxy(0, 24);
    printf("Up/Down - Move | Esc - Exit");

    if (kbhit()) {
      ch = getch();

      if (ch == 0 || ch == 224) {
        ch = getch();

        switch (ch) {
	  case 72: y -= 5; break;  // up
	  case 80: y += 5; break;  // down
	}
      }

      if (ch == 27) break;
    }

    // doesn't exist on MS C 7.0
    // delay(30);

  }

  // getch();

  // back to text mode
  SCREEN(0x10);

  return 0;
}
