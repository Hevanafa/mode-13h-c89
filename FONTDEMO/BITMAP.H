typedef unsigned char byte;

typedef struct {
  byte** data;
  int w;
  int h;
} Bitmap;

void load_bmp(Bitmap *bitmap, char* filename);
void debug_bmp(Bitmap *bitmap);
void draw_bmp(Bitmap *bitmap, int x, int y);
void draw_region(Bitmap *bitmap, int srcx, int srcy, int w, int h, int dx, int dy);
void draw_str(Bitmap *bitmap, char* text, int x, int y);
