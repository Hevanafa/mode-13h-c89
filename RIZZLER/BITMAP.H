typedef unsigned char byte;

typedef struct {
  byte** data;
  int w;
  int h;
  int is_set;
} Bitmap;

void load_bmp(Bitmap *bitmap, byte* filename);
void debug_bmp(Bitmap *bitmap);
void draw_bmp(Bitmap *bitmap, int x, int y);

void draw_char(Bitmap *font, byte c, int x, int y);
void draw_str(Bitmap *font, byte* text, int x, int y);
