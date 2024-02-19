#include<dos.h>
#include<stdio.h>

// 13-02-2024

#define VIDSEG 0xa000
// screen width & height
#define SW 320
#define SH 200

typedef unsigned char byte;

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

void PIX(int x, int y, byte colour) {
	if (x < 0 || x >= 320 || y < 0 || y >= 320) return;

	// for some reason, VIDSEG makes it throw an "Expression syntax" error.
	pokeb(VIDSEG, (y * SW) + x, colour);
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
	Bitmap score_spr;

	// Be sure to change the DIR first before loading the image
	LoadBMP(&score_spr, "SCORESM2.BMP");
	// DebugBMP(&score_spr);

	// start mode 13h just like in QBASIC / VBDOS
	SCREEN(0x13);

	printf("Hello");

	DrawBMP(&score_spr, 10, 10);

	getch();

	// back to text mode
	SCREEN(0x10);

	return 0;
}