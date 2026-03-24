#include "tgaimage.h"
#include <cmath>
#include <cstdlib>

int a,b;
  
  
void BresenhamLine(int ax, int ay, int bx, int by, TGAImage &framebuffer,
                  TGAColor color) {
  for (float t = 0.0f; t < 1.0f; t += 0.02f) {
    int new_x = static_cast<int>(lround(ax + (bx - ax) * t));
    int new_y = static_cast<int>(lround(ay + (by - ay) * t));
    framebuffer.set(new_x, new_y, color);
  }
}

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer,
          TGAColor color) {
  bool steep = std::abs(ax - bx) < std::abs(ay - by);
  if (steep) { // if the line is steep, we transpose the image
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) {
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  for (int x = ax; x <= bx; x++) {
    float t = (x - ax) / static_cast<float>(bx - ax);
    int y = static_cast<int>(lround(ay + (by - ay) * t));
    if (steep) // if transposed, de−transpose
      framebuffer.set(y, x, color);
    else
      framebuffer.set(x, y, color);
  }
}

void line_enhanced(int ax, int ay, int bx, int by, TGAImage &framebuffer,
                  TGAColor color) {
  bool steep = std::abs(ax - bx) < std::abs(ay - by);

  if (steep) { // if the line is steep, we transpose the image
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) { // make it left−to−right
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  int y = ay;
  int ierror = 0;
  for (int x = ax; x <= bx; x++) {
    if (steep) // if transposed, de−transpose
      framebuffer.set(y, x, color);
    else
      framebuffer.set(x, y, color);
    ierror += 2 * std::abs(by - ay);
    y += (by > ay ? 1 : -1) * (ierror > bx - ax);
    ierror -= 2 * (bx - ax) * (ierror > bx - ax);
  }
}

void line_enhanced_optimized(int ax, int ay, int bx, int by, 
                  TGAImage &framebuffer, TGAColor color) {
  int dx = abs(bx - ax);
  int dy = abs(by - ay);
  int sx = (bx > ax) ? 1 : -1;
  int sy = (by > ay) ? 1 : -1;
  int err = dx - dy;
  int e2 = 0;

  int steps = std::max(dx, dy);

  for (int i = 0; i <= steps; i++) {
      framebuffer.set(ax, ay, color);

      e2 = err << 1;

      int maskX = -(e2 > -dy);
      int maskY = -(e2 < dx);

      ax += sx & maskX;
      ay += sy & maskY;

      err += (-dy & maskX) + (dx & maskY);
  }
}

void line_enhanced_fast(int ax, int ay, int bx, int by,
                        TGAImage &framebuffer, TGAColor color) {
  bool steep = std::abs(ax - bx) < std::abs(ay - by);

  if (steep) {
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) {
    std::swap(ax, bx);
    std::swap(ay, by);
  }

  const int dx = bx - ax;
  const int dy_abs = std::abs(by - ay);
  const int y_step = (by > ay) ? 1 : -1;

  int y = ay;
  int err = 0;

  if (steep) {
    for (int x = ax; x <= bx; ++x) {
      framebuffer.set(y, x, color);
      err += dy_abs;
      if ((err << 1) > dx) {
        y += y_step;
        err -= dx;
      }
    }
  } else {
    for (int x = ax; x <= bx; ++x) {
      framebuffer.set(x, y, color);
      err += dy_abs;
      if ((err << 1) > dx) {
        y += y_step;
        err -= dx;
      }
    }
  }
}