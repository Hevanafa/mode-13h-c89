typedef unsigned char byte;

typedef struct {
  byte** data;
  int w;
  int h;
} Bitmap;

void LoadBMP(Bitmap *bitmap, char* filename);
void DrawBMP(Bitmap *bitmap, int x, int y);
