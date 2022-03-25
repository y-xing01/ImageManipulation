//
// Created by floodd on 22/03/2022.
//

// Created by floodd on 11/02/2022.
//


#ifndef SDLTEST_TGA_FILEHANDLER_H
#define SDLTEST_TGA_FILEHANDLER_H
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
struct Rgb
{
    Rgb() : r(0.5), g(0.5), b(0)  {}
    Rgb(float rr) : r(rr), g(rr), b(rr) {}
    Rgb(float rr, float gg, float bb) : r(rr), g(gg), b(bb) {}
    float r, g, b;
};

class Image {



    Image(const unsigned int _w, const unsigned int _h) :
            w(_w), h(_h), pixels(nullptr)
    {
        pixels = new Rgb[w * h];

    }

    unsigned int w, h;
    Rgb *pixels;
public:
    Image() : w(300), h(400)
    {
        pixels = new Rgb[w * h];
    }
    void savePPM(string filename);
    void load(string filename);
    void scale(float amnt);
    void greyScale();
    void inverse();
    void flipHorizontal();
    void flipVertically();
    void filterRed();
    void filterGreen();
    void filterBlue();
    void AdditionalFunction1();
    void AdditionalFunction2();
    void AdditionalFunction3();
    void rotate90cw();
    Rgb* getImage();
    int getWidth();
    int getHeight();
    ~Image() { if (pixels != nullptr) delete [] pixels; }

};



#endif //SDLTEST_TGA_FILEHANDLER_H

