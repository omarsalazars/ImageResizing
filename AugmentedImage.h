#ifndef AugmentedImage_H
#define AugmentedImage_H
#include "CImg.h"

using namespace cimg_library;

class AugmentedImage : public CImg<unsigned char>{
    using CImg<unsigned char>::CImg;
    using CImg<unsigned char>::operator=;
    using CImg<unsigned char>::operator();
    public:
        AugmentedImage(CImg<unsigned char> img) : CImg<unsigned char>(img){};
        CImg<unsigned char> EmptySpace();
        CImg<unsigned char> NearestNeighbor();
        CImg<unsigned char> Average(CImg<unsigned char> i1, CImg<unsigned char> i2);
        CImg<unsigned char> NLagrangeInterpolation(int grade);
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