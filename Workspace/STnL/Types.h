typedef unsigned Color;

#define COLOR_ARGB(a, r, g, b) (((unsigned)(unsigned char)(a) << 24) | ((unsigned)(unsigned char)(r) << 16) | ((unsigned short)(unsigned char)(g) << 8) | (unsigned char)(b))
#define COLOR_RGB(r, g, b) COLOR_ARGB(255, r, g, b)
