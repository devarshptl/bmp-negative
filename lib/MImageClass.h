//
// Created by Devarsh Patel on 4/3/23.
//

#ifndef BLOCKAPPS_MIMAGECLASS_H
#define BLOCKAPPS_MIMAGECLASS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>

using namespace std;

// constants
const int MIN_RGB = 0;
const int MAX_RGB = 255;
const int INITIALS_LENGTH = 2;
const int FAIL = 1;
const int SUCCESS = 0;

// 'BM' initials
struct bmp_initials {
    u_char initials[INITIALS_LENGTH];
};

// BMP header
struct bmp_header {
    ::uint32_t size;
    ::uint16_t c1;
    ::uint16_t c2;
    ::uint32_t offset;
};

// BMP DIB information
struct bmp_dib {
    ::uint32_t header_size;
    ::int32_t width;
    ::int32_t height;
    ::uint16_t planes;
    ::uint16_t bits_per_pixel;
    ::uint32_t cmp;
    ::uint32_t byte_size;
    ::int32_t x_res;
    ::int32_t y_res;
    ::uint32_t colors;
    ::uint32_t imp_colors;
};

// pixel color
class Pixel {
public:
    int r, g, b;
    Pixel(): r(0), g(0), b(0) {}
    Pixel(int red, int green, int blue): r(red), g(green), b(blue) {}
};


// define pixel vector
typedef vector<vector<Pixel>> PixelMatrix;

class MImageClass {
private:
    int maxValue;
    PixelMatrix pixels;
public:
    MImageClass(const string&);
    MImageClass(int mv, PixelMatrix p): maxValue(mv), pixels(p){}
    bool isValid();
    MImageClass negative();
    void save(const string&);
};


#endif //BLOCKAPPS_MIMAGECLASS_H
