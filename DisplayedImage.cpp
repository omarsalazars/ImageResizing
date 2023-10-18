#include "CImg.h"
#include "DisplayedImage.h"
#include <string>

using namespace std;
using namespace cimg_library;

DisplayedImage::DisplayedImage(CImg<unsigned char> image, string imageName, string displayName){
    this->image = image;
    this->imageName = imageName;
    this->displayName = displayName;
}