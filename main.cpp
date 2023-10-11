#define cimg_use_jpeg
#define cimg_use_png
#include "CImg.h"
#include "AugmentedImage.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace cimg_library;
using namespace std;

const char* instructions = R"(Usage:
./interpolation <image_path> <initialX> <initialY> <finalX> <finalY> [incrementX] [incrementY]

Params:
char* image_path - Relative or absolute path to the image.
int initialX - X axis coordinate of the upper-left corner of rectangle. Default value: 100.
int initialY - Y axis coordinate of the upper-left corner of rectangle. Default value: 100.
int finalX - X axis coordinate of the bottom-right corner of rectangle. Should be greater than initialX. Default value: 200.
int finalY - Y axis coordinate of the bottom right corner of rectangle. Should be greater than initialY. Default value: 200.
int incrementX - X axis increment of the image. Default value: 4 <optional>
int incrementY - Y axis increment of the image. Default value: 4 <optional>

    InitialX/Y
        ----------------
        [              ]
        ----------------
                        FinalX/Y
)";

CImg<unsigned char> originalImage;
AugmentedImage augmentedImage;

const char* defaultImage = "Imagen1/img.jpg";
int defaultInitialX = 100;
int defaultInitialY = 100;
int defaultFinalX = 120;
int defaultFinalY = 120;
int defaultIncrementX = 4;
int defaultIncrementY = 4;

void die(const char* errorMessage){
    cerr << errorMessage << endl;
    cerr << instructions << endl;
    exit(1);
}

void init(int argc, char* argv[])
{
    int size = argc;

    if(size >= 2){
        if(strcmp(argv[1], "--help") == 0){
            cout << instructions << endl;
            exit(0);
        }
    }

    if(size != 6 && size != 8 && size != 1){
        die("Incorrect number of arguments.");
    }

    const char* filename = size == 1 ? defaultImage : argv[1];
    int initialX = defaultInitialX;
    int initialY = defaultInitialY;
    int finalX = defaultFinalX;
    int finalY = defaultFinalY;
    int incrementX = defaultIncrementX;
    int incrementY = defaultIncrementY;

    originalImage.assign(filename);

    // Parse arguments
    if(size >= 6){
        // Validate arguments are in range
        if(atoi(argv[2])<0 || atoi(argv[3])<0 || atoi(argv[4])<0 || atoi(argv[5])<0) 
            die("All arguments must be poisitive.");
        
        initialX = atoi(argv[2]);
        initialY = atoi(argv[3]);
        finalX = atoi(argv[4]);
        finalY = atoi(argv[5]);

        if(initialX > originalImage.width()) die("initialX must be shorter than max image width");
        if(initialY > originalImage.height()) die("initialY must be shorter than max image height");
        if(finalX > originalImage.width()) die("finalX must be shorter than max image width");
        if(finalY > originalImage.height()) die("finalY must be shorter than max image height");
        if(finalX < initialX) die("finalX must be shorter than initialX");
        if(finalY < initialY) die("finalY must be shorter than initialY");

        if(size == 8){
            if(atoi(argv[6])<0 || atoi(argv[7])<0)
                die("All arguments must be positive.");
            incrementX = atoi(argv[6]);
            incrementY = atoi(argv[7]);
        }
    }

    augmentedImage = (AugmentedImage)originalImage.get_crop( initialX, initialY, finalX, finalY );
    augmentedImage.setIncrementX(incrementX);
    augmentedImage.setIncrementY(incrementY);

    cout << "Width: " << originalImage.width() << endl;
    cout << "Height: " << originalImage.height() << endl;
    cout << "Spectrum: " << originalImage.spectrum() << endl;

    cout << "cropped Width: " << augmentedImage.width() << endl;
    cout << "cropped Height: " << augmentedImage.height() << endl;
    cout << "cropped Spectrum: " << augmentedImage.spectrum() << endl;
}

int main(int argc, char* argv[])
{
    init(argc, argv);

    CImg<unsigned char> grade2 = augmentedImage.NLagrangeInterpolation(2);
    CImg<unsigned char> grade3 = augmentedImage.NLagrangeInterpolation(3);

    CImgDisplay main_disp(originalImage,"Original");
    CImgDisplay draw_dispA(augmentedImage, "Augmented");
    CImgDisplay draw_dispB(grade2, "Lagrange Interpolation grade 2");
    CImgDisplay draw_dispC(grade3, "Lagrange Interpolation grade 3");
    CImgDisplay draw_dispD(augmentedImage.NLagrangeInterpolation(4), "Lagrange Interpolation grade 4");
    CImgDisplay draw_dispE(augmentedImage.NearestNeighbor(), "Nearest Neighbor");
    CImgDisplay draw_dispF(augmentedImage.EmptySpace(), "Expanded");
    CImgDisplay draw_dispG(augmentedImage.Average(grade2, grade3), "Interpolation grade 2 and 3 average");

    /*bilineal.save("bilineal.png");
    bicubica.save("bicubica.png");
    nearest.save("nearest.png");
    promedio.save("promedio.png");
    quinta.save("quinta.png");
    decima.save("decima.png");
    aumentada->save("aumentada.png");
    region->save("aux.png");*/

    while (!main_disp.is_closed()){
        main_disp.wait();
    }

    return 0;
}
