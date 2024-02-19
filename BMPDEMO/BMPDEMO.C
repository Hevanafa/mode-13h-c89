#include<dos.h>
#include<stdio.h>
#include<conio.h>
#include<math.h>
// #include<fstream>
// #include<iostream>

// 12-02-2024
// This freezes the system for some reason.

#define VIDSEG 0xa000
#define SW 320
#define SH 200

typedef unsigned char byte;
byte *buffer;

typedef struct {
	byte **data;
	int w;
	int h;
} Bitmap;

void SCREEN(byte mode) {
	_AH = 0;
	_AL = mode;

	geninterrupt(0x10);
}

void init_buffer() {
	// out of memory
	buffer = (byte *) malloc(320*200);

	if (buffer == 0) {
		printf("Out of memory.");
		return;
	}

	memset(buffer, 0, 320*200);
}

void PIX(int x, int y, byte colour) {
	if (x < 0 || x >= 320 || y < 0 || y >= 320) return;

	// for some reason, VIDSEG makes it throw an "Expression syntax" error.
	pokeb(VIDSEG, (y * SW) + x, colour);
	// buffer[y * 320 + x] = colour;
}

void flush() {
	int x, y;

	for (y = 0; y < 200; y++)
	for (x = 0; x < 320; x++)
		pokeb(VIDSEG, (y * SW) + x, buffer[y * 320 + x]);
}

void rect(int x, int y, int w, int h, byte colour) {
	int a, b;

	for (b = y; b <= y + h; b++)
	for (a = x; a <= x + w; a++)
		PIX(a, b, colour);
}

void rectb(int x, int y, int w, int h, byte colour) {
	int a, b;

	for (b = y; b <= y + h; b++) {
		PIX(x, b, colour);
		PIX(x + w, b, colour);
	}

	for (a = x; a <= x + w; a++) {
		PIX(a, y, colour);
		PIX(a, y + h, colour);
	}
}

void LoadBMP(Bitmap *bitmap, char* filename) {
	byte buf[2];
	int w = 1, h = 1;
	int pad = 0, palsize = 0;
	int start;
	int byteidx;
	int a, b;
	int pair;

	FILE *fh;

	free(bitmap->data);

	fh = fopen(filename, "rb");

	fseek(fh, 18, SEEK_SET);
	fread(buf, 1, 1, fh);
	w = buf[0];
	bitmap->w = w;

	fseek(fh, 22, SEEK_SET);
	fread(buf, 1, 1, fh);
	h = buf[0];
	bitmap->h = h;

	**bitmap->data = malloc(h * sizeof(*bitmap->data));
	for (a = 0; a < h; a++)
		bitmap->data[a] = (byte*) malloc(w * sizeof(*bitmap->data[a]));

	fseek(fh, 50, SEEK_SET);
	fread(buf, 1, 2, fh);
	palsize = buf[1] * 256 + buf[0];
	pad = (4 - w % 4) % 4;

	start = 53 + palsize * 4;

	for (b = 0; b < h; b++)
	for (a = 0; a < w; a++) {
		byteidx = start + (b * (w + pad)) + a;

		fseek(fh, byteidx, SEEK_SET);
		fread(buf, 1, 1, fh);
		bitmap->data[h - b - 1][a] = buf[0];
	}

	fclose(fh);
}


void DebugBMP(Bitmap *bitmap) {
	int a, b;

	printf("w: %d, h: %d\n", bitmap->w, bitmap->h);

	// printf("palsize: %d\n", palsize);

	for (b = 0; b < bitmap->h; b++) {
		for (a = 0; a < bitmap->w; a++)
			printf("%d ", bitmap->data[b][a]);
		printf("\n");
	}

	getch();
}

void DrawBMP(Bitmap *bitmap, int x, int y) {
	int a, b;

	for (b = 0; b < bitmap->h; b++)
		for (a = 0; a < bitmap->w; a++)
			PIX(x + a, y + b, bitmap->data[b][a]);
}

int main() {
	int a, b, ch;
	int x = 50, y = 50;
	long frames = 0;

	Bitmap score_spr;

	LoadBMP(&score_spr, "SCORESM2.BMP");
	DebugBMP(&score_spr);
	
	// Note: the program freezes ??
	
	return;

	// start mode 13h just like in QBASIC / VBDOS
	SCREEN(0x13);

	// still has contents
	// DebugBMP(&score_spr);

	// init_buffer();

	// LoadBMP(&score_spr, "SCORESM2.BMP");

	// This is either blank or skipped at this point
	// DebugBMP(&score_spr);


	while (1) {
		// DrawBMP(&score_spr, 10, 10);

		rect(x - 5, y - 5, 15, 15, 0);
		rect(x, y, 5, 5, y / 5 * 16 + x / 5);
		rectb(x, y, 5, 5, 15);

		flush();

		// This doesn't output anything
		//gotoxy(20, 1);
		//textcolor(WHITE);
		//printf("Esc - Exit\n");
		frames++;

		if (kbhit()) {
			ch = getch();

			if (ch == 0 || ch == 224) {
				ch = getch();

				switch (ch) {
					case 72: // up
						y -= 5;
					break;

					case 80: // down
						y += 5;
					break;

					case 75:
						x -= 5;
					break;

					case 77:
						x += 5;
				}
			}

			if (ch == 27) break;

		}


	}

	getch();

	// back to text mode
	SCREEN(0x10);

	return 0;
}