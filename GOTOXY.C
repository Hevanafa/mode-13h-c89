#include<stdio.h>
#include<dos.h>

// DOS BIOS gotoxy() demonstration
// 15-02-2024

typedef unsigned char byte;
typedef unsigned char uint;

union REGS regs;
byte activepage;
byte columns;
byte screenmode;

void getmode() {
  regs.h.ah = 15;
  int86(0x10, &regs, &regs);
}

void color(uint colour) {
  getmode();

  regs.h.ah = 9;
  regs.h.bh = activepage;
  regs.x.cx = 1;
  regs.h.al = screenmode & 7;
  regs.h.bl = colour;

  int86(0x10, &regs, &regs);
}

void gotoxy(uint x, uint y) {
  getmode();

  regs.h.ah = 2;
  regs.h.dl = x;
  regs.h.dh = y;
  regs.h.bh = activepage;

  int86(0x10, &regs, &regs);
}


int main() {
  gotoxy(0, 24);
  color(15);
  printf("Hello, I'm at the bottom!");

  gotoxy(0, 10);
  color(1);
  printf("Blue");
  color(2);
  printf("Green");
  getch();

}
