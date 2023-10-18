#ifndef AugmentedImage_H
#define AugmentedImage_H
#include "CImg.h"
#include "AugmentedImage.h"
#include "DisplayedImage.h"
#include "MathUtil.h"
#include <iostream>
#include <string>

using namespace cimg_library;

class AugmentedImage : public CImg<unsigned char>{
    using CImg<unsigned char>::CImg;
    using CImg<unsigned char>::operator=;
    using CImg<unsigned char>::operator();
    public:
        AugmentedImage(CImg<unsigned char> img) : CImg<unsigned char>(img){};
        DisplayedImage EmptySpace();
        DisplayedImage NearestNeighbor();
        DisplayedImage Average(CImg<unsigned char> i1, CImg<unsigned char> i2);
        DisplayedImage NLagrangeInterpolation(int grade);
        int newWidth();
        int newHeight();
        void setIncrementX(int incrementX);
        void setIncrementY(int incrementY);
        int getIncrementX();
        int getIncrementY();
        void dumpProps();
    private:
        int incrementX;
        int incrementY;
};

#endif