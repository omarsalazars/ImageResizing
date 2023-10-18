#ifndef DisplayedImage_H
#define DisplayedImage_H

#include "CImg.h"
#include <string>

using namespace std;
using namespace cimg_library;

class DisplayedImage{
    public:
        CImg<unsigned char> image;
        string imageName;
        string displayName;
        DisplayedImage(CImg<unsigned char> image, string imageName, string displayName);
};

#endif