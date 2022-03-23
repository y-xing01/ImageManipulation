//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"

int Image::getWidth()
{
    return w;
}
int Image::getHeight()
{
    return h;
}
int getHeight();

Rgb* Image::getImage()
{
    return pixels;
}
void Image::rotate90cw()
{
    Rgb *tempArr = new Rgb[w*h];

    for(int y =0; y < h; y++)
    {
        for(int x = 0; x < w;x++)
        {
            int r = ((x * h) + (h - y -1));
            //[c ] [ m - r - 1];

            tempArr[r] = pixels[y*w+x];
        }
    }
    pixels = tempArr;
    int t= w;
    w= h;
    h=t;
}
void Image::filterRed()
{
    for(int y = 0 ; y < h; y++)
    {
        for(int x = 0 ; x < w; x++)
        {
            pixels[y*w+x].g=0;
            pixels[y*w+x].b=0;

        }
    }
}
void Image::filterGreen()
{
    for(int y = 0 ; y < h; y++)
    {
        for(int x = 0 ; x < w; x++)
        {
            pixels[y*w+x].r=0;

            pixels[y*w+x].b=0;

        }
    }
}
void Image::filterBlue()
{
    for(int y = 0 ; y < h; y++)
    {
        for(int x = 0 ; x < w; x++)
        {
            pixels[y*w+x].r=0;
            pixels[y*w+x].g=0;

        }
    }
}


void Image::blur()
{
    float rTotal = 0, gTotal=0, bTotal=0;
    for(int y = 0 ; y < h; y++)
    {
        for(int x = 0 ; x < w; x++)
        {
            rTotal = 0; gTotal=0; bTotal=0;
            if(x==0||y==0||x==w-1||y==h-1)
                continue;
            else
            {
                Rgb neighbours[] = {pixels[(y-1)*w+(x-1)],pixels[(y-1)*w+x],pixels[(y-1)*w+x+1]
                                    ,pixels[(y)*w+x-1],pixels[(y)*w+x],pixels[(y)*w+x+1],
                                    pixels[(y+1)*w+(x-1)],pixels[(y+1)*w+x],pixels[(y+1)*w+x+1]};

                for(Rgb p: neighbours)
                {
                    rTotal += p.r;
                    gTotal += p.g;
                    bTotal += p.b;

                }
                pixels[y*w+x].r=rTotal/9;
                pixels[y*w+x].g=gTotal/9;
                pixels[y*w+x].b=bTotal/9;


            }

        }
    }
}
void Image::scale(float amnt)
{
    int newW = w*(amnt);
    int newH = h*(amnt);
    float stepX = w/(double)newW;
    float stepY = h/(double)newH;

    Rgb *temp = new Rgb[newW*newH];
    float currY, currX;
    currX = currY = 0;
    for(int y = 0; y< newH;y++)
    {
        currX=0;
        for(int x = 0; x< newW;x++)
        {
            int nextX = (int)currX;
            int nextY = (int)currY;
            //cout << nextX << ", " << nextY<<endl;
            temp[y*newW+x] = pixels[nextY*w+nextX];
            currX+=stepX;
        }
        currY+=stepY;
    }
    pixels = temp;
    w = newW;
    h=newH;
}
void Image::greyScale()
{
    for(int y = 0 ; y < h; y++)
    {
        for(int x = 0 ; x < w; x++)
        {
            int col = (pixels[y*w+x].r*255+pixels[y*w+x].g*255+pixels[y*w+x].b*255)/3.0f;
            pixels[y*w+x].r=col;
            pixels[y*w+x].g=col;
            pixels[y*w+x].b=col;

        }
    }
}
void Image::flipHorizontal()
{

    Rgb temp;
    for(int y = 0 ; y < h; y++) {
        for (int x = 0; x < (w/2); x++) {
            temp = pixels[y*w+x];
            pixels[y*w+x] = pixels[(y*w)+(w-x)];
            pixels[(y*w)+(w-x)] = temp;
        }
    }
    // cout << "Flop"<<endl;
}
void Image::flipVertically()
{

    Rgb temp;
    for(int y = 0 ; y < h/2; y++) {
        for (int x = 0; x < (w); x++) {

            temp = pixels[y*w+x];
            pixels[y*w+x] = pixels[((h-1-y)*w)+x];
            pixels[((h-1-y)*w)+x] = temp;
        }
    }
}
void Image::inverse()
{
    for(int y = 0 ; y < h; y++)
    {
        for(int x = 0 ; x < w; x++)
        {
            pixels[y*w+x].r=1-pixels[y*w+x].r;
            pixels[y*w+x].g=1-pixels[y*w+x].g;
            pixels[y*w+x].b=1-pixels[y*w+x].b;

        }
    }
}
void Image::savePPM(string filename)
{
    if (this->w == 0 || this->h == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << this->w << " " <<  this->h << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i <  this->w *  this->h; ++i) {
            r = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].r) * 255);
            g = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].g) * 255);
            b = static_cast<unsigned char>(std::min(1.f,  this->pixels[i].b) * 255);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}

void Image::load(string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
    try {
        if (ifs.fail()) { throw("Can't open input file"); }
        std::string header;
        int w, h, b;
        ifs >> header;
        if (strcmp(header.c_str(), "P6") != 0) throw("Can't read input file");
        ifs >> w >> h >> b;
        this->w = w; this->h = h;
        this->pixels = new Rgb[w * h]; // this is throw an exception if bad_alloc
        ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data
        unsigned char pix[3];
        // read each pixel one by one and convert bytes to floats
        for (int i = 0; i < w * h; ++i) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0] / 255.f;
            this->pixels[i].g = pix[1] / 255.f;
            this->pixels[i].b = pix[2] / 255.f;
        }
        ifs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ifs.close();
    }


}
