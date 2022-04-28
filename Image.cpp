//
// Created by floodd on 23/03/2022.
//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include "Image.h"
#include <valarray>

bool Image::load(string filename) {
    ifstream ifs(filename, std::ios::binary);
    string magicNumber;
    ifs >> magicNumber;
    ifs >> this->w;
    ifs >> this->h;
    int p;
    ifs >> p;
    ifs.ignore(256, '\n');
    if (p == 255) {
        delete[] pixels;
        pixels = new Rgb[w * h];
        unsigned char pix[3];
        for (int i = 0; i < w * h; i++) {
            ifs.read(reinterpret_cast<char *>(pix), 3);
            this->pixels[i].r = pix[0];
            this->pixels[i].g = pix[1];
            this->pixels[i].b = pix[2];
        }
        return true;
    }
    return false;

}

bool Image::loadRaw(string filename) {
    ifstream in(filename);

    if (in.good()) {
        in >> w;
        in >> h;

        for (int i = 0; i < w * h; ++i) {
            float r, g, b;
            in >> r >> g >> b;
            pixels[i].r = pow(r, 1 / 2.2) * 255;
            pixels[i].g = pow(r, 1 / 2.2) * 255;
            pixels[i].b = pow(r, 1 / 2.2) * 255;
            cout << r << pixels[i].r << endl;
        }
        in.close();

    }
    return false;
}

bool Image::savePPM(string filename) {
    if (this->w == 0 || this->h == 0) {
        return false;
    }
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) throw ("Can't open output file");
        ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->w * this->h; i++) {
            r = static_cast<unsigned char>(std::min(1.f, (float) this->pixels[i].r) * this->pixels[i].r);
            g = static_cast<unsigned char>(std::min(1.f, (float) this->pixels[i].g) * this->pixels[i].g);
            b = static_cast<unsigned char>(std::min(1.f, (float) this->pixels[i].b) * this->pixels[i].b);
            ofs << r << g << b;
        }
        ofs.close();
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
    return false;
}


void Image::filterRed() {
    for (int i = 0; i < w * h; ++i) {
        this->pixels[i].g = 0;
        this->pixels[i].b = 0;
    }
}

void Image::filterGreen() {
    for (int i = 0; i < w * h; ++i) {
        this->pixels[i].r = 0;
        this->pixels[i].b = 0;
    }
}

void Image::filterBlue() {
    for (int i = 0; i < w * h; ++i) {
        this->pixels[i].r = 0;
        this->pixels[i].g = 0;
    }
}

void Image::greyScale() {
    for (int i = 0; i < w * h; i++) {
        unsigned char greyscaleValue = (this->pixels[i].r + this->pixels[i].g + this->pixels[i].b) / 3;
        this->pixels[i].r = greyscaleValue;
        this->pixels[i].g = greyscaleValue;
        this->pixels[i].b = greyscaleValue;
    }
}

void Image::flipHorizontal() {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w / 2; x++) {
            Rgb temp = pixels[x + y * w];
            pixels[x + y * w] = pixels[y * w + (w - x)];
            pixels[y * w + (w - x)] = temp;
        }
    }
}

void Image::flipVertically() {
    for (int y = 0; y < h / 2; y++) {
        for (int x = 0; x < w; x++) {
            Rgb temp = pixels[x + y * w];
            pixels[x + y * w] = pixels[x + (h - 1 - y) * w];
            pixels[x + (h - 1 - y) * w] = temp;
        }
    }
}

void Image::AdditionalFunction1() {
    //Rotate by 90
    Rgb *temp = new Rgb[w * h];
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int a = x * h + (h - 1 - y);
            temp[a] = pixels[y * w + x];
        }
    }
    pixels = temp;
    int tempHeight = h;
    h = w;
    w = tempHeight;
    temp = nullptr;
    delete[] temp;
}

void Image::AdditionalFunction2() {
    //Inverting color
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Rgb temp = pixels[x + y * w];
            pixels[x + y * w].r = 255 - temp.r;
            pixels[x + y * w].g = 255 - temp.g;
            pixels[x + y * w].b = 255 - temp.b;
        }
    }
}

void Image::AdditionalFunction3(int newX, int newY, int newWidth, int newHeight) {
    Image *cropImage = new Image[newWidth, newHeight];
    for (int y = 0; y < newHeight; y++) {
        if ((y + newY) > h) {
            break;
        }
        for (int x = 0; x < newWidth; x++) {
            if ((x + newX) > w) {
                break;
            }
            memcpy(&cropImage->pixels[(x + y * newWidth)], &this->pixels[(x + newX + (y + newY) * w)], 3);
        }
    }
    w = newWidth;
    h = newHeight;

    delete this->pixels;
    this->pixels = cropImage->pixels;
    cropImage = nullptr;
}


void Image::AdvancedFeature() {

}


/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth() {
    return w;
}

int Image::getHeight() {
    return h;
}

Rgb *Image::getImage() {
    return pixels;
}