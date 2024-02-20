#include<dos.h>  // REGS

// screen width & height
#define SW 320
#define SH 200

typedef unsigned char byte;
typedef unsigned int uint;

extern byte far *VGA = (byte far*)0xa0000000L;
extern byte buffer[23040];

// extern union REGS regs;
extern byte activepage;
extern byte columns;
extern byte screenmode;

void CLS();
void SCREEN(byte mode);
void PSET(int x, int y, byte colour);
void fill_buf(byte colour);
void flush_buf();
void getmode();
void gotoxy(uint x, uint y);
void rect(int x, int y, int w, int h, byte colour);

// the same as safefree from Wikipedia
void delete(void* ptr);
