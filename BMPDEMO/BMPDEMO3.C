#include<dos.h>
#include<conio.h>
#include<stdio.h>
#include<time.h>

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
	if (colour == 13) return;

	pokeb(VIDSEG, (y * SW) + x, colour);
}


void rect(int x, int y, int w, int h, byte colour) {
	int a, b;

	for (b = y; b <= y + h; b++)
	for (a = x; a <= x + w; a++)
		PIX(a, b, colour);
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

void DrawRegion(Bitmap *bitmap, int srcx, int srcy, int w, int h, int dx, int dy) {
	int a, b;

	for (b = 0; b < h; b++)
	for (a = 0; a < w; a++) {
		if (srcy + b >= bitmap->h ||
			srcx + a >= bitmap->w)
			continue;

		PIX(dx + a, dy + b, bitmap->data[srcy + b][srcx + a]);
	}
}

int main() {
	int a;
	Bitmap score_spr;
	clock_t begin = clock(), end;
	double time_spent;


	// Be sure to change the DIR first before loading the image
	LoadBMP(&score_spr, "SCORESM2.BMP");
	// DebugBMP(&score_spr);

	// start mode 13h just like in QBASIC / VBDOS
	SCREEN(0x13);

	printf("Hello");

	// DrawBMP(&score_spr, 10, 10 * a);
	for (a = 0; a <= 50; a++) {
		DrawRegion(&score_spr, (a % 10) * 6, 0, 6, 7, 10 + (a * 6), 10);
	}

	// Stackoverflow: questions/5248915
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	gotoxy(1, 6);
	printf("%.f ms", time_spent * 1000);

	getch();

	// back to text mode
	SCREEN(0x10);

	return 0;
}