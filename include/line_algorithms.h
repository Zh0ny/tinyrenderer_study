#include "tgaimage.h"
using namespace std;

void BresenhamLine(int ax, int ay, int bx, int by, TGAImage &framebuffer,
                    TGAColor color);

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, 
            TGAColor color);

void line_enhanced(int ax, int ay, int bx, int by, TGAImage &framebuffer, 
            TGAColor color); 

void line_enhanced_optimized(int ax, int ay, int bx, int by, 
              TGAImage &framebuffer, TGAColor color);

void line_enhanced_fast(int ax, int ay, int bx, int by,
              TGAImage &framebuffer, TGAColor color);