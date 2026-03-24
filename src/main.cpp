#include "tgaimage.h"
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <line_algorithms.h>
#include <string>
using namespace std;


constexpr TGAColor white{{255, 255, 255, 255}};
constexpr TGAColor green{{0, 255, 0, 255}};
constexpr TGAColor red{{0, 0, 255, 255}};
constexpr TGAColor blue{{255, 128, 64, 255}};
constexpr TGAColor yellow{{0, 200, 255, 255}};

int main() {
  try {
    constexpr int width{64};
    constexpr int height{64};
    TGAImage framebuffer(width, height, TGAImage::RGB);
    int ax{7}, ay{3};
    int bx{12}, by{37};
    int cx{62}, cy{53};

    line(ax, ay, bx, by, framebuffer, blue);
    line(cx, cy, bx, by, framebuffer, green);
    line(cx, cy, ax, ay, framebuffer, yellow);
    line(ax, ay, cx, cy, framebuffer, red);

    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, red);
    framebuffer.set(cx, cy, yellow);

    std::srand(static_cast<unsigned int>(std::time({})));
    for (int i=0; i<(1<<12); i++) {
        int a_x = rand()%width, a_y = rand()%height;
        int b_x = rand()%width, b_y = rand()%height;
        line(a_x, a_y, b_x, b_y, framebuffer, 
            TGAColor{{ 
                    static_cast<std::uint8_t>(std::rand() % 256),  
                    static_cast<std::uint8_t>(std::rand() % 256), 
                    static_cast<std::uint8_t>(std::rand() % 256), 
                    static_cast<std::uint8_t>(std::rand() % 256)
        }});
    }   

    framebuffer.write_tga_file("framebuffer3.tga");
    cout<<"finish"<<endl;
    cout.flush();  // força flush
    cerr.flush();  // força stderr também
  } catch (const std::exception& e) {
    cerr << "Exception: " << e.what() << endl;
    return 1;
  } catch (...) {
    cerr << "Unknown exception" << endl;
    return 1;
  }
  return 0;
}