#include<stdio.h>  // printf
#include<dos.h>    // REGS
#include<conio.h>  // getch
#include<time.h>   // time

#define KESC 0x01
#define KUP 0x48
#define KRIGHT 0x4d
#define KDOWN 0x50
#define KLEFT 0x4b

// By Hevanafa, 16-02-2024
// This can be compiled with Microsoft C++ 7.0 (not Visual C++).
// Requires Large/Huge memory model to run.


#define SW 320
#define SH 200

typedef unsigned char byte;
typedef unsigned int uint;


// segment A000:0000
byte far *VGA = (byte far*)0xa0000000L;

union REGS regs;	// programming model 8088
// union REGS inregs, outregs;

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

void delay(long ms) {
  double s = ms / 1000.0;
  clock_t start_t = clock();
  while ((double) (clock() - start_t) / CLOCKS_PER_SEC < s);
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

int iskeydown(int keycode) {
  union REGS inregs, outregs;

  while (kbhit()) getch();

  inregs.h.ah = 0x11;
  int86(0x16, &inregs, &outregs);

  return (outregs.h.al & keycode) != 0;
}


int main() {
  int a, b, ch;
  int y = 50;
  int t;
  long frames = 0;
  int done = 0;

  // for FPS counter
  time_t now = time(0);
  struct tm *timeinfo = localtime(&now);
  long last_t = timeinfo->tm_sec;
  int last_fps = 0, fps = 0;

  int colour;

  SCREEN(0x13);


  while (!done) {
    // outregs.h.al still returns 224
    // Found out that outregs.h.al becomes 224 after pressing the arrow key
    if (iskeydown(KESC)) done = 1;

    // if (iskeydown(KUP))	 y -= 3;
    // if (iskeydown(KDOWN)) y += 3;


    // update
    t = (t + 1) % 24;
    frames++;

    // 10 seconds
    if (frames > 200) break;

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
    gotoxy(0, 22);
    // printf("last_t: %d", last_t);
    printf("FPS: %d, %d", last_fps, frames);

    // gotoxy(0, 23);
    // printf("%d %d", outregs.h.al, outregs.h.al & KESC);


    gotoxy(0, 24);
    printf("Up/Down - Move | Esc - Exit");


    // custom implementation
    delay(10);
  }

  // getch();

  // back to text mode
  SCREEN(0x10);

  return 0;
}
