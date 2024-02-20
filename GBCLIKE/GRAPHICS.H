// screen width & height
#define VGAW 320
#define VGAH 200
#define SW 160
#define SH 144

typedef unsigned char byte;
typedef unsigned int uint;

byte far *VGA = (byte far*)0xa0000000L;
byte buffer[23040];

union REGS regs;
byte activepage;
byte columns = 79;
byte screenmode = 2;

void CLS() {
  memset(buffer, 0, (long) SW * SH);
}

void SCREEN(byte mode) {
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
    PSET(a, b, colour);
}

// the same as safefree from Wikipedia
void delete(void* ptr) {
  free(ptr);
  ptr = 0;
}
