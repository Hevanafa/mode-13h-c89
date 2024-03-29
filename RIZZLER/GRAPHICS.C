#include<stdlib.h>
#include "GRAPHICS.H"

// redeclaration, must be the same as in the header file
byte buffer[23040];

byte activepage;
byte columns = 79;
byte screenmode = 2;

void CLS() {
  memset(buffer, 0, (long) SW * SH);
}

void SCREEN(byte mode) {
  union REGS regs;
  regs.h.ah = 0;
  regs.h.al = mode;

  int86(0x10, &regs, &regs);
}

void PSET(int x, int y, byte colour) {
  if (x < 0 || x >= SW || y < 0 || y >= SH) return;
  if (colour == 13) return;

  buffer[(long)y * SW + x] = colour;
}

void fill_buf(byte colour) {
  int a;
  for (a = 0; a < SW * SH; a++)
    buffer[a] = colour;
}

void flush_buf() {
  int a, b, pos;

  for (b = 0; b < SH; b++)
  for (a = 0; a < SW; a++) {
    pos = b * SW + a;
    VGA[(28 + b) * VGAW + 80 + a] = buffer[b * SW + a];
  }
}

void getmode(union REGS *regs) {
  regs->h.ah = 15;
  int86(0x10, regs, regs);

  activepage = regs->h.bh;
  screenmode = regs->h.al;
  columns = regs->h.ah;
}

void gotoxy(uint x, uint y) {
  union REGS regs;
  getmode(&regs);

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
    PSET(a, b, colour);
}

// the same as safefree from Wikipedia
void delete(void* ptr) {
  free(ptr);
  ptr = 0;
}
