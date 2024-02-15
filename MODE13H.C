#include<stdio.h>
#include<dos.h>

// This can be compiled with Microsoft C++ 7.0 (not Visual C++)
// 14-02-2024


#define SW 320
#define SH 200

typedef unsigned char byte;

// segment A000:0000
byte far *VGA = (byte far*)0xa0000000L;

void SCREEN(byte mode) {
  union REGS regs;

  regs.h.ah = 0;     // set the video mode
  regs.h.al = mode;

  int86(0x10, &regs, &regs);
}

void PSET(int x, int y, byte colour) {
  if (x < 0 || x >= 320 || y < 0 || y >= 320) return;

  // write directly to the video memory
  VGA[y * SW + x] = colour;
}

void rect(int x, int y, int w, int h, byte colour) {
  int a, b;

  for (b = y; b < y + h; b++)
  for (a = x; a < x + w; a++)
    PSET(a, b, colour);
}

int main() {
  int a, b;

  SCREEN(0x13);

  printf("Hello\n");

  for (b = 0; b < 16; b++)
  for (a = 0; a < 16; a++)
    // PSET(a, b + 30, (byte)(b * 16 + a));
    rect(a * 8, (b + 2) * 8, 8, 8, (byte)(b * 16 + a));

  // printf("Press any key to exit.\n");
  getch();

  // back to text mode
  SCREEN(0x10);

  return 0;
}
